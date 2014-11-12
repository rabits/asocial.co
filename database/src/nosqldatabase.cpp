#include "nosqldatabase.h"

// Version
#define DATABASE_VERSION 0x0001
// Backward-compatibility with all versions greater than
#define DATABASE_MINIMAL_VERSION 0x0001

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
    options.compression = leveldb::kNoCompression;
    if( ! leveldb::DB::Open(options, db_path.toUtf8().constData(), &m_db).ok() ) {
        qWarning() << "Unable to open database" << db_path;
        return;
    }

    // Check database version
    qDebug() << "Database version:" << fetchStore("database_version", QByteArray::number(DATABASE_VERSION)).toLong();
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
