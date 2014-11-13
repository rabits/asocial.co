#include "database/sqldatabase.h"

#include <QDebug>
#include <QDir>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlDriver>

#include "qsql_sqlcipher_p.h"

SqlDatabase::SqlDatabase(QObject *parent, QString name, QString path)
    : QObject(parent)
    , m_name(name)
    , m_path(path)
{
    qDebug() << "Create SqlDatabase" << m_name;
}

SqlDatabase::~SqlDatabase()
{
    m_db.close();
    qDebug() << "Destroy SqlDatabase" << m_name;
}

void SqlDatabase::open(const QString &password)
{
    if( m_db.isOpen() )
        return;

    if( m_path.isEmpty() || m_name.isEmpty() ) {
        qCritical() << "Unable to open DB with empty path or file:" << m_path << m_name;
        return;
    }

    QDir(m_path).mkpath(".");
    QString db_path = m_path + "/" + m_name;
    qDebug() << "Open SQL DB:" << db_path;

    m_db = QSqlDatabase::addDatabase(new QSQLCipherDriver(), "frontend");
    m_db.setDatabaseName(db_path);
    m_db.setPassword(password);

    if( ! m_db.open() ) {
        qCritical() << m_db.lastError();
        qFatal("Unable to open sql database");
    }
}

void SqlDatabase::table(const QString &name, const QStringList &fields)
{
    qDebug() << "Table query:" << name;
    QSqlQuery query(m_db);

    if( ! query.exec(QString("CREATE TABLE IF NOT EXISTS %1 (%2)").arg(m_db.driver()->escapeIdentifier(name, QSqlDriver::TableName), fields.join(','))) )
        qCritical() << m_db.lastError();
}

void SqlDatabase::backup()
{
    qDebug("Starting SqlDatabase backup");

    m_db.close();

    QString backup_name = m_path + "/backup_" + m_name;

    // Remove previous backup
    if( QFile::exists(backup_name) )
        QFile::remove(backup_name);

    // Copy current database to backup
    if( ! QFile::copy(m_path + "/" + m_name, backup_name) ) {
        QFile::remove(backup_name);
        qFatal("Unable to create backup of the database");
    }

    qDebug("Database backup done");

    if( ! m_db.open() ) {
        qCritical() << m_db.lastError();
        qFatal("Unable to open sql database");
    }
}
