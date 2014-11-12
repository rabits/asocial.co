#ifndef SQLDATABASE_H
#define SQLDATABASE_H

#include <QObject>
#include <QtSql/QSqlDatabase>

class SqlDatabase
    : public QObject
{
    Q_OBJECT

public:
    explicit SqlDatabase(QObject *parent = 0, QString name = "", QString path = "");
    ~SqlDatabase();

    void open(const QString &password = "");
    void table(const QString &name, const QStringList &fields);

private:
    QString m_name;
    QString m_path;

    QSqlDatabase m_db;
};

#endif // SQLDATABASE_H
