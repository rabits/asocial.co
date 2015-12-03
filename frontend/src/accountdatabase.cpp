#include "accountdatabase.h"

// Version
#define DATABASE_VERSION 1
// Backward-compatibility with all versions greater than
#define DATABASE_MINIMAL_VERSION 0

#include "crypto/crypto.h"

#include <QDebug>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QJsonDocument>
#include <QDateTime>

AccountDatabase::AccountDatabase(QObject *parent, QString name, QString path)
    : SqlDatabase(parent, name, path)
    , m_version(-1)
{
    qDebug() << "Creating Account Database" << name;
}

void AccountDatabase::open(const QString &password)
{
    SqlDatabase::open(password);

    // Check version
    if( version() == 0 ) {
        qDebug("Start initialiation of the database");
        upgrade(version());
    } else if( version() < DATABASE_MINIMAL_VERSION ) {
        qCritical() << "Version of opened database" << version() << "is too old (minimum supported is" << DATABASE_MINIMAL_VERSION << ")";
        qFatal("Database is too old");
    } else if( version() > DATABASE_VERSION ) {
        qCritical() << "Version of opened database" << version() << "is greater than current client version" << DATABASE_VERSION;
        qFatal("Database is too new");
    } else if( version() < DATABASE_VERSION ) {
        qWarning() << "Your database version" << version() << "will be upgraded to" << DATABASE_VERSION;
        qWarning() << "If you find some issues - you always can restore previous version of application and database backup";
        backup();
        upgrade(version());
    }
}

int AccountDatabase::createProfile(const QJsonObject &profile)
{
    qDebug("Creating new profile");

    QSqlQuery query(m_db);
    query.prepare("INSERT INTO profiles (date, address, data, overlay, description) VALUES (:date, :address, :data, :overlay, :description)");
    query.bindValue(":date", QDateTime::currentDateTime().toTime_t());
    query.bindValue(":address", profile.value("address").toString());
    query.bindValue(":data", QJsonDocument(profile.value("data").toObject()).toJson(QJsonDocument::Compact));
    query.bindValue(":overlay", QJsonDocument(profile.value("overlay").toObject()).toJson(QJsonDocument::Compact));
    query.bindValue(":description", profile.value("description").toString());

    if( ! query.exec() )
        qCritical() << query.lastError();

    return query.lastInsertId().toInt();
}

QJsonObject AccountDatabase::getProfile(const int id)
{
    qDebug("Getting profile");

    QSqlQuery query(m_db);
    QJsonObject out;

    query.prepare("SELECT rowid as id, date, address, data, overlay, description FROM profiles WHERE rowid = :id LIMIT 1");
    query.bindValue(":id", id);

    if( ! query.exec() ) {
        qCritical() << query.lastError();
    } else if( query.next() ) {
        out["id"] = query.value("id").toInt();
        out["date"] = query.value("date").toLongLong();
        out["address"] = query.value("address").toString();
        out["data"] = QJsonDocument::fromJson(query.value("data").toByteArray()).object();
        out["overlay"] = QJsonDocument::fromJson(query.value("overlay").toByteArray()).object();
        out["description"] = query.value("description").toString();
    } else {
        qWarning() << "Unable to find required profile id#" << id;
    }

    return out;
}

bool AccountDatabase::updateProfileData(const QJsonObject &profile)
{
    qDebug("Updating profile data");

    QSqlQuery query(m_db);

    query.prepare("UPDATE profiles SET date = :date, data = :data WHERE rowid = :id");
    query.bindValue(":id", profile.value("id").toInt());
    query.bindValue(":date", QDateTime::currentDateTime().toTime_t());
    query.bindValue(":data", QJsonDocument(profile.value("data").toObject()).toJson(QJsonDocument::Compact));

    if( ! query.exec() ) {
        qCritical() << query.lastError();
        return false;
    }

    return true;
}

QString AccountDatabase::createAddress()
{
    PrivKey* key = Crypto::I()->genKey();
    QString address = key->getPubKey()->getAddress();

    storeKey(key, "Key for primary account profile");

    delete key;

    return address;
}

int AccountDatabase::storeKey(const PrivKey *key, const QString description)
{
    qDebug() << "Store address" << key->getPubKey()->getAddress();

    QSqlQuery query(m_db);
    query.prepare("INSERT INTO keys (address, pubkey, privkey, privkey_encrypted, description) VALUES (:address, :pubkey, :privkey, :privkey_encrypted, :description)");
    query.bindValue(":address", key->getPubKey()->getAddress());
    query.bindValue(":pubkey", QVariant::fromValue(key->getPubKey()->getData()));
    query.bindValue(":privkey", QVariant::fromValue(key->getData()));
    query.bindValue(":privkey_encrypted", QVariant::fromValue(key->isEncrypted()));
    query.bindValue(":description", description);

    if( ! query.exec() )
        qCritical() << query.lastError();

    return query.lastInsertId().toInt();
}

long AccountDatabase::version()
{
    if( m_version != -1 )
        return m_version;

    qDebug("Get database version");

    // Prepare table before query it
    table("database", QStringList()
          << "version int not null"
          << "description text not null");

    QSqlQuery query(m_db);
    if( ! query.exec("SELECT version FROM database ORDER BY rowid DESC LIMIT 1") ) {
        qCritical() << query.lastError();
    } else {
        query.next();
        if( ! query.isNull(0) ) {
            m_version = query.value(0).toLongLong();
        } else {
            // Database no version - it's empty
            m_version = 0;
        }
    }

    return m_version;
}

void AccountDatabase::setVersion(const long version, const QString &description)
{
    qDebug() << "Set database version" << version;

    QSqlQuery query(m_db);
    query.prepare("INSERT INTO database (version, description) VALUES (:version, :description)");
    query.bindValue(":version", QVariant::fromValue(version));
    query.bindValue(":description", description);

    if( ! query.exec() )
        qCritical() << query.lastError();

    m_version = version;
}

void AccountDatabase::upgrade(const long from_version)
{
    qDebug() << "Start database upgrade from version" << from_version;

    QSqlQuery query(m_db);

    // Changes in versions should be applied consistently
    switch( from_version + 1 ) {
        case 1:
            table("keys", QStringList()
                  << "address text not null"
                  << "pubkey blob not null"
                  << "privkey blob"
                  << "privkey_encrypted integer"
                  << "description text");

            table("devices", QStringList()
                  << "date integer not null"    // Last update date
                  << "name text not null"       // Name of device
                  << "address text not null"    // Bitcoin device address
                  << "data text not null"       // Json with device key id, picture etc
                  << "description text");       // Description for device

            table("profiles", QStringList()     // Contains profiles of users
                  << "date integer not null"    // Last update date
                  << "address text not null"    // Bitcoin account address
                  << "data text not null"       // Json with profile data
                  << "overlay text not null"    // Json overlay profile data
                  << "description text not null");

            table("profiles_history", QStringList() // History of profiles
                  << "date integer not null"    // Change date
                  << "data text not null"       // Changed data
                  << "description text");       // Description of change

            table("storage_type", QStringList()
                  << "name text not null");     // Readable name of storage data type
            query.exec("INSERT INTO media_type (name) VALUES ('image'), ('audio'), ('video'), ('file')");

            table("storage_metadata", QStringList()
                  << "date integer not null"    // Last update date
                  << "type int not null"        // Type of media
                  << "owner_id int not null"    // Data owner
                  << "url text not null"        // URL to find media
                  << "data text not null");     // Json with metadata

            table("storage_data", QStringList()
                  << "data blob not null");     // Private place for storage data, is managed by InternalStorage

            setVersion(1, "First version of database");
        default:
            qDebug() << "Upgrade done";
    }
}

