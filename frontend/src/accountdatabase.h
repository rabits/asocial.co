#ifndef ACCOUNTDATABASE_H
#define ACCOUNTDATABASE_H

#include "database/sqldatabase.h"

#include "crypto/privkey.h"

#include <QJsonObject>
#include <QJsonArray>

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

    Q_INVOKABLE int createEvent(const QJsonObject &event);
    Q_INVOKABLE QJsonObject getEvent(const int id);
    Q_INVOKABLE QJsonArray getEvents(const int type);
    Q_INVOKABLE bool updateEvent(const QJsonObject &event);

    Q_INVOKABLE QString getEventTypeName(const int id);
    Q_INVOKABLE int getEventTypeId(const QString &name);
    Q_INVOKABLE QJsonArray getEventTypes();

    Q_INVOKABLE QString createAddress();

    Q_INVOKABLE int storeKey(const PrivKey* key, const QString description);

private:
    long version();
    void setVersion(const long version, const QString &description);
    void upgrade(const long from_version);

    long m_version; // Current database version
};

#endif // ACCOUNTDATABASE_H
