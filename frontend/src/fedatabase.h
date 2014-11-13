#ifndef FEDATABASE_H
#define FEDATABASE_H

#include "database/sqldatabase.h"

class FEDatabase
    : public SqlDatabase
{
    Q_OBJECT

public:
    FEDatabase(QObject *parent, QString name, QString path);

    void open(const QString &password);
    long version();
    void setVersion(const long version, const QString &description);

    void upgrade(const long from_version);

private:
    long m_version; // Current database version
};

#endif // FEDATABASE_H
