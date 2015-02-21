#ifndef FEDATABASE_H
#define FEDATABASE_H

#include "database/sqldatabase.h"

#include <QJsonObject>
#include <QJsonArray>

class PrivKey;
class AccountDatabase;

class FEDatabase
    : public SqlDatabase
{
    Q_OBJECT

public:
    FEDatabase(QObject *parent, QString name, QString path);
    ~FEDatabase();

    void open(const QString &password);

    QJsonArray getAccounts();
    int createAccount(const QJsonObject &account, const QString &password);
    bool openAccount(const int id);
    void closeAccount();

    AccountDatabase* getCurrentAccount();

private:
    long version();
    void setVersion(const long version, const QString &description);
    void upgrade(const long from_version);

    PrivKey* getAccountPassKey(const int id);

    long m_version; // Current database version

    AccountDatabase* m_active_account;
};

#endif // FEDATABASE_H
