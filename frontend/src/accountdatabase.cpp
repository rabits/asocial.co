#include "accountdatabase.h"

// Version
#define DATABASE_VERSION 3
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

qint64 AccountDatabase::createProfile(const QJsonObject &profile)
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

    return query.lastInsertId().toLongLong();
}

QJsonObject AccountDatabase::getProfile(const qint64 id)
{
    qDebug("Getting profile");

    QSqlQuery query(m_db);
    QJsonObject out;

    query.prepare("SELECT rowid, date, address, data, overlay, description FROM profiles WHERE rowid = :id LIMIT 1");
    query.bindValue(":id", QVariant::fromValue(id));

    if( ! query.exec() ) {
        qCritical() << query.lastError();
    } else if( query.next() ) {
        out["id"] = query.value("rowid").toLongLong();
        out["date"] = query.value("date").toLongLong();
        out["address"] = query.value("address").toString();
        out["data"] = QJsonDocument::fromJson(query.value("data").toByteArray()).object();
        out["overlay"] = QJsonDocument::fromJson(query.value("overlay").toByteArray()).object();
        out["description"] = query.value("description").toString();
    } else
        qWarning() << "Unable to find required profile id#" << id;

    return out;
}

bool AccountDatabase::updateProfileData(const QJsonObject &profile)
{
    qDebug("Updating profile data");

    QSqlQuery query(m_db);

    query.prepare("UPDATE profiles SET date = :date, data = :data, overlay = :overlay WHERE rowid = :id");
    query.bindValue(":id", profile.value("id").toInt());
    query.bindValue(":date", QDateTime::currentDateTime().toTime_t());
    query.bindValue(":data", QJsonDocument(profile.value("data").toObject()).toJson(QJsonDocument::Compact));
    query.bindValue(":overlay", QJsonDocument(profile.value("overlay").toObject()).toJson(QJsonDocument::Compact));

    if( ! query.exec() ) {
        qCritical() << query.lastError();
        return false;
    }

    return true;
}

qint64 AccountDatabase::createEvent(const QJsonObject &event)
{
    qDebug("Creating new event");

    int type = getEventTypeId(event.value("type").toString());

    if( type != -1 ) {
        QSqlQuery query(m_db);
        query.prepare("INSERT INTO events (date, occur, link, type, owner, recipient, data, overlay) VALUES (:date, :occur, :link, :type, :owner, :recipient, :data, :overlay)");
        query.bindValue(":date", QDateTime::currentDateTime().toTime_t());
        query.bindValue(":occur", event.value("occur").toVariant().toLongLong());
        query.bindValue(":link", event.value("link").toVariant().toLongLong());
        query.bindValue(":type", type);
        query.bindValue(":owner", event.value("owner").toVariant().toLongLong());
        query.bindValue(":recipient", event.value("recipient").toVariant().toLongLong());
        query.bindValue(":data", QJsonDocument(event.value("data").toObject()).toJson(QJsonDocument::Compact));
        query.bindValue(":overlay", QJsonDocument(event.value("overlay").toObject()).toJson(QJsonDocument::Compact));

        if( ! query.exec() )
            qCritical() << query.lastError();

        return query.lastInsertId().toLongLong();
    }

    qWarning() << "Event type" << event.value("type").toString() << "not found";
    return -1;
}

QJsonObject AccountDatabase::getEvent(const qint64 id)
{
    qDebug("Getting event");

    QSqlQuery query(m_db);
    QJsonObject out;

    query.prepare("SELECT rowid, date, occur, link, type, owner, recipient, data, overlay FROM events WHERE rowid = :id LIMIT 1");
    query.bindValue(":id", QVariant::fromValue(id));

    if( ! query.exec() )
        qCritical() << query.lastError();
    else if( query.next() ) {
        out["id"] = query.value("rowid").toInt();
        out["date"] = query.value("date").toLongLong();
        out["occur"] = query.value("occur").toLongLong();
        out["type"] = getEventTypeName(query.value("type").toInt());
        out["owner"] = query.value("owner").toLongLong();
        out["recipient"] = query.value("recipient").toLongLong();
        out["data"] = QJsonDocument::fromJson(query.value("data").toByteArray()).object();
        out["overlay"] = QJsonDocument::fromJson(query.value("overlay").toByteArray()).object();
    } else
        qWarning() << "Unable to find required profile id#" << id;

    return out;
}

QJsonArray AccountDatabase::getEvents(const qint64 occur_from, const qint64 occur_to, const int type, const qint64 owner, const qint64 recipient)
{
    qDebug("Getting events");
    QJsonArray out;

    QString query_string = "SELECT rowid, date, occur, link, type, owner, recipient, data, overlay FROM events WHERE occur >= :from AND occur <= :to";
    QSqlQuery query(m_db);

    if( type > -1 )
        query_string.append(" AND type = :type");
    if( owner > -1 )
        query_string.append(" AND owner = :owner");
    if( recipient > -1 )
        query_string.append(" AND recipient = :recipient");

    query.prepare(query_string);

    if( type > -1 )
        query.bindValue(":type", type);
    if( owner > -1 )
        query.bindValue(":owner", QVariant::fromValue(owner));
    if( recipient > -1 )
        query.bindValue(":recipient", QVariant::fromValue(recipient));

    query.bindValue(":from", QVariant::fromValue(occur_from));
    query.bindValue(":to", QVariant::fromValue(occur_to));

    if( ! query.exec() )
        qCritical() << query.lastError();
    else {
        while( query.next() ) {
            QJsonObject obj;

            obj["id"] = query.value("rowid").toInt();
            obj["date"] = query.value("date").toLongLong();
            obj["occur"] = query.value("occur").toLongLong();
            obj["type"] = getEventTypeName(query.value("type").toInt());
            obj["owner"] = query.value("owner").toLongLong();
            obj["recipient"] = query.value("recipient").toLongLong();
            obj["data"] = QJsonDocument::fromJson(query.value("data").toByteArray()).object();
            obj["overlay"] = QJsonDocument::fromJson(query.value("overlay").toByteArray()).object();

            out.append(obj);
        }
    }

    return out;
}

bool AccountDatabase::updateEvent(const QJsonObject &event)
{
    qDebug("Updating event data");

    QSqlQuery query(m_db);

    query.prepare("UPDATE events SET date = :date, occur = :occur, data = :data, overlay = :overlay WHERE rowid = :id");
    query.bindValue(":id", event.value("id").toInt());
    query.bindValue(":date", QDateTime::currentDateTime().toTime_t());
    query.bindValue(":occur", event.value("occur").toVariant().toLongLong());
    query.bindValue(":data", QJsonDocument(event.value("data").toObject()).toJson(QJsonDocument::Compact));
    query.bindValue(":overlay", QJsonDocument(event.value("overlay").toObject()).toJson(QJsonDocument::Compact));

    if( ! query.exec() ) {
        qCritical() << query.lastError();
        return false;
    }

    return true;
}

QString AccountDatabase::getEventTypeName(const int id)
{
    QSqlQuery query(m_db);
    query.prepare("SELECT name FROM event_type WHERE rowid = :id LIMIT 1");
    query.bindValue(":id", id);

    if( ! query.exec() )
        qCritical() << query.lastError();

    query.next();
    if( ! query.isNull(0) )
        return query.value(0).toString();

    // If found nothing
    return "";
}

int AccountDatabase::getEventTypeId(const QString &name)
{
    QSqlQuery query(m_db);
    query.prepare("SELECT rowid FROM event_type WHERE name = :name LIMIT 1");
    query.bindValue(":name", name);

    if( ! query.exec() )
        qCritical() << query.lastError();

    query.next();
    if( ! query.isNull(0) )
        return query.value(0).toInt();

    // If not found
    return -1;
}

QJsonArray AccountDatabase::getEventTypes()
{
    qDebug("Getting event types");
    QJsonArray out;

    QSqlQuery query(m_db);

    if( ! query.exec("SELECT rowid, name, description FROM event_types") )
        qCritical() << query.lastError();
    else {
        while( query.next() ) {
            QJsonObject obj;

            obj["id"] = query.value("rowid").toInt();
            obj["date"] = query.value("name").toString();
            obj["occur"] = query.value("description").toString();

            out.append(obj);
        }
    }

    return out;
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

    // Default version db set
    m_version = 0;

    // Prepare table before query it
    table("database", QStringList()
          << "version int not null"
          << "description text not null");

    QSqlQuery query(m_db);
    if( ! query.exec("SELECT version FROM database ORDER BY rowid DESC LIMIT 1") )
        qCritical() << query.lastError();

    query.next();
    if( ! query.isNull(0) )
        m_version = query.value(0).toLongLong();

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
            query.exec("INSERT INTO storage_type (name) VALUES ('image'), ('audio'), ('video'), ('file')");

            table("storage_metadata", QStringList()
                  << "date integer not null"    // Last update date
                  << "type int not null"        // Type of media
                  << "owner_id int not null"    // Data owner
                  << "url text not null"        // URL to find media
                  << "data text not null");     // Json with metadata

            table("storage_data", QStringList()
                  << "data blob not null");     // Private place for storage data, is managed by InternalStorage

            setVersion(1, "First version of database");
        case 2:
            table("event_type", QStringList()
                  << "name text not null"       // Readable name of event type
                  << "description text not null"); // Info about event type
            query.exec("INSERT INTO event_type (name, description) VALUES "
                       "('message', 'p2p messages'),"
                       "('fact', 'some information')");

            table("events", QStringList()
                  << "date integer not null"    // Last update date
                  << "occur integer not null"   // Date when event occur/occurred
                  << "link integer"             // Link to another event id
                  << "type int not null"        // Type of event
                  << "owner int not null"       // Profile owns event
                  << "recipient int"            // Recipient of the event
                  << "data text not null"       // Json with event data
                  << "overlay text not null");  // Json overlay data

            query.exec("DROP TABLE storage_type");
            table("storage_type", QStringList()
                  << "name text not null"       // Readable name of storage data type
                  << "description text not null"); // Info about storage type
            query.exec("INSERT INTO storage_type (name, description) VALUES "
                       "('image', 'Static graphical data like jpeg, png, svg etc. - just a picture'),"
                       "('media', 'Any audio or video media data like mp3, mkv, apng etc. - moving picture w/wo audio'),"
                       "('3d', 'Mesh, textures and animation data like stl, obj etc.'),"
                       "('file', 'Just an unknown binary file like bin, zip, tar.gz etc.')");

            setVersion(2, "Added events table to separate events and profile data");
        case 3:
            query.exec("DROP TABLE profiles_history");
            table("profiles_history", QStringList() // History of profiles
                  << "id integer not null"      // Id of the profile
                  << "date integer not null"    // Change date
                  << "data text not null"       // Changed data
                  << "description text");       // Description of change

            table("events_history", QStringList() // History of events
                  << "id integer not null"      // Id of the event
                  << "date integer not null"    // Change date
                  << "data text not null"       // Changed data
                  << "description text");       // Description of change

            setVersion(3, "Added forgottent history id for profile & history for events");
        default:
            qDebug() << "Upgrade done";
    }
}
