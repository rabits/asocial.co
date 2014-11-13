#include "fedatabase.h"

// Version
#define DATABASE_VERSION 1
// Backward-compatibility with all versions greater than
#define DATABASE_MINIMAL_VERSION 0

#include <QDebug>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QStringList>

FEDatabase::FEDatabase(QObject *parent, QString name, QString path)
    : SqlDatabase(parent, name, path)
    , m_version(-1)
{
}

void FEDatabase::open(const QString &password)
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

long FEDatabase::version()
{
    if( m_version != -1 )
        return m_version;

    qDebug("Get database version");

    // We need to prepare table before query it
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

void FEDatabase::setVersion(const long version, const QString &description)
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

void FEDatabase::upgrade(const long from_version)
{
    qDebug() << "Start database upgrade from version" << from_version;

    QSqlQuery query(m_db);

    // Changes in versions should be applied consistently
    switch( from_version + 1 ) {
        case 1:
            // Still no changes
            setVersion(1, "First version of database");
        default:
            qDebug() << "Upgrade done";
    }
}
