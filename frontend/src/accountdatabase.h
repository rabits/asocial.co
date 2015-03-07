#ifndef ACCOUNTDATABASE_H
#define ACCOUNTDATABASE_H

#include "database/sqldatabase.h"

#include <QJsonObject>

class AccountDatabase
    : public SqlDatabase
{
    Q_OBJECT

public:
    AccountDatabase(QObject *parent, QString name, QString path);

    void open(const QString &password);

    Q_INVOKABLE int createProfile(const QJsonObject &profile);
    Q_INVOKABLE QJsonObject getProfile(const int id);
    Q_INVOKABLE bool updateProfileData(const QJsonObject &profile);

private:
    long version();
    void setVersion(const long version, const QString &description);
    void upgrade(const long from_version);

    long m_version; // Current database version
};

#endif // ACCOUNTDATABASE_H
