#include "database/nosqldatabase.h"

#include <QDebug>
#include <QDir>

#include "leveldb/db.h"

NoSqlDatabase::NoSqlDatabase(QObject *parent, QString name, QString path)
    : QObject(parent)
    , m_name(name)
    , m_path(path)
    , m_db(NULL)
{
    qDebug() << "Create NoSqlDatabase" << m_name;
}

NoSqlDatabase::~NoSqlDatabase()
{
    delete m_db;
    qDebug() << "Destroy NoSqlDatabase" << m_name;
}

void NoSqlDatabase::open()
{
    if( m_db != NULL )
        return;

    if( m_path.isEmpty() || m_name.isEmpty() ) {
        qCritical() << "Unable to open DB with empty path or file:" << m_path << m_name;
        return;
    }

    QDir(m_path).mkpath(".");
    QString db_path = m_path + "/" + m_name;
    qDebug() << "Open NoSQL DB:" << db_path;

    leveldb::Options options;
    options.create_if_missing = true;
    if( ! leveldb::DB::Open(options, db_path.toUtf8().constData(), &m_db).ok() ) {
        qWarning() << "Unable to open database" << db_path;
        return;
    }
}

void NoSqlDatabase::close()
{
    qDebug() << "Close NoSQL DB:" << m_name;
    delete m_db;
}

QByteArray NoSqlDatabase::fetchStore(const QString &key, const QByteArray &val)
{
    qDebug("Fetching with store data");
    ::std::string value(val.constData());
    if( ! m_db->Get(leveldb::ReadOptions(), key.toStdString(), &value).ok() ) {
        qDebug() << "Unable to get value" << key;
        store(key, val);
    }

    return QByteArray::fromRawData(value.c_str(), value.size());
}

void NoSqlDatabase::store(const QString &key, const QByteArray &val)
{
    std::string value(val.constData());
    m_db->Put(leveldb::WriteOptions(), key.toStdString(), value);
}

void NoSqlDatabase::backup()
{
    qDebug("Starting NoSqlDatabase backup");

    close();

    QString backup_from = m_path + "/" + m_name;
    QDir backup_name(m_path + "/backup_" + m_name);

    // Remove previous backup
    if( backup_name.exists() )
        backup_name.remove(".");

    backup_name.mkpath(".");
    QStringList files = QDir(backup_from).entryList(QDir::Files);
    foreach( const QString &file, files ) {
        if( ! QFile::copy(backup_from + "/" + file, backup_name.path() + "/" + file) ) {
            backup_name.remove(".");
            qFatal("Unable to create backup of the database");
        }
    }

    qDebug("Database backup done");

    open();
}
