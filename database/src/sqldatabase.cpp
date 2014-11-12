#include "sqldatabase.h"

// Version
#define DATABASE_VERSION 0x0001
// Backward-compatibility with all versions greater than
#define DATABASE_MINIMAL_VERSION 0x0001

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

    // Check database version
    //qDebug() << "Database version:" << fetchStore("database_version", QByteArray::number(DATABASE_VERSION)).toLong();

}

void SqlDatabase::table(const QString &name, const QStringList &fields)
{
    qDebug() << "Table query:" << name;
    QSqlQuery query(m_db);
    query.prepare(QString("CREATE TABLE IF NOT EXISTS %1 (%2)").arg(m_db.driver()->escapeIdentifier(name, QSqlDriver::TableName), fields.join(',')));

    if( ! query.exec() )
        qCritical() << m_db.lastError();
}
