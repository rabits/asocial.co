#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>

class Network;
class Database;

class Backend
    : public QObject
{
    Q_OBJECT

public:
    explicit Backend(QObject *parent = 0);
    ~Backend();

    void init();
private:
    Database *m_database;
    Network  *m_network;
};

#endif // BACKEND_H
