#ifndef NOSQLDATABASE_H
#define NOSQLDATABASE_H

#include <QObject>

namespace leveldb {
    class DB;
}

class NoSqlDatabase
    : public QObject
{
    Q_OBJECT

public:
    explicit NoSqlDatabase(QObject *parent = 0, QString name = "", QString path = "");
    ~NoSqlDatabase();

    void open();
    QByteArray fetchStore(const QString &key, const QByteArray &val = "");
    void store(const QString &key, const QByteArray &val = "");

private:
    QString m_name;
    QString m_path;

    leveldb::DB *m_db;
};

#endif // NOSQLDATABASE_H
