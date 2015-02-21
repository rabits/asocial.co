#include "accountdatabase.h"

// Version
#define DATABASE_VERSION 1
// Backward-compatibility with all versions greater than
#define DATABASE_MINIMAL_VERSION 0

#include <QDebug>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QJsonDocument>

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
    query.prepare("INSERT INTO profiles (address, data, overlay, description) VALUES (:address, :data, :overlay, :description)");
    query.bindValue(":address", QVariant::fromValue(profile.value("address")));
    query.bindValue(":data", QJsonDocument(profile.value("data").toObject()).toJson(QJsonDocument::Compact));
    query.bindValue(":overlay", QJsonDocument(profile.value("overlay").toObject()).toJson(QJsonDocument::Compact));
    query.bindValue(":description", QVariant::fromValue(profile.value("description")));

    if( ! query.exec() )
        qCritical() << query.lastError();

    return query.lastInsertId().toInt();
}

QJsonObject AccountDatabase::getProfile(const int id)
{
    qDebug("Getting profile");

    QSqlQuery query(m_db);
    QJsonObject out;

    query.prepare("SELECT rowid as id, address, data, overlay, description FROM profiles WHERE rowid = :id LIMIT 1");
    query.bindValue(":id", id);

    if( ! query.exec() ) {
        qCritical() << m_db.lastError();
    } else if( query.next() ) {
        out["id"] = query.value("id").toInt();
        out["address"] = query.value("address").toString();
        out["data"] = QJsonDocument::fromJson(query.value("data").toByteArray()).object();
        out["overlay"] = QJsonDocument::fromJson(query.value("overlay").toByteArray()).object();
        out["description"] = query.value("description").toString();
    } else {
        qWarning() << "Unable to find required profile id#" << id;
    }

    return out;
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
        qCritical() << m_db.lastError();
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
        qCritical() << m_db.lastError();

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
                  << "pubkey blob not null"
                  << "privkey blob"
                  << "privkey_encrypted integer"
                  << "description text");
            table("devices", QStringList()
                  << "name text not null"
                  << "address text not null"
                  << "data text not null"        // Json with device key id, picture etc
                  << "description text");
            table("profiles", QStringList()      // Contains profiles of users
                  << "address text not null"
                  << "data text not null"        // Json with profile data
                  << "overlay text not null"     // Json overlay profile data
                  << "description text not null");
            table("history", QStringList()
                  << "date integer not null"
                  << "data text not null"
                  << "description text");
            setVersion(1, "First version of database");
        default:
            qDebug() << "Upgrade done";
    }
}

