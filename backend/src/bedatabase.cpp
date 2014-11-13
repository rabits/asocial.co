#include "bedatabase.h"

// Version
#define DATABASE_VERSION 1
// Backward-compatibility with all versions greater than
#define DATABASE_MINIMAL_VERSION 0

#include <QDebug>

BEDatabase::BEDatabase(QObject *parent, const QString &name, const QString &path)
    : NoSqlDatabase(parent, name, path)
    , m_version(-1)
{
}

void BEDatabase::open()
{
    NoSqlDatabase::open();

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

long BEDatabase::version()
{
    if( m_version != -1 )
        return m_version;

    qDebug("Get database version");

    // Check database version
    m_version = fetchStore("database_version", 0).toLong();

    return m_version;
}

void BEDatabase::setVersion(const long version)
{
    qDebug() << "Set database version" << version;

    store("database_version", QByteArray::number((qlonglong)version));

    m_version = version;
}

void BEDatabase::upgrade(const long from_version)
{
    qDebug() << "Start database upgrade from version" << from_version;

    // Changes in versions should be applied consistently
    switch( from_version + 1 ) {
        case 1:
            // Still no changes
            setVersion(1);
        default:
            qDebug() << "Upgrade done";
    }
}
