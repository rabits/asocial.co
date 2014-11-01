#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>

class Database
    : public QObject
{
    Q_OBJECT

public:
    explicit Database(QObject *parent = 0);
    Database(QString name, QString path, QObject *parent = 0);
    ~Database();

private:
    QString m_name;
    QString m_path;
};

#endif // DATABASE_H
