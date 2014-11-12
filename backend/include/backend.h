#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>

class QTimer;

class Network;
class NoSqlDatabase;

class Backend
    : public QObject
{
    Q_OBJECT

public:
    explicit Backend(QObject *parent = 0);
    ~Backend();

    void init();

public slots:
    void broadcast();

private:
    NoSqlDatabase *m_database;
    Network  *m_network;

    QTimer   *m_broadcast_timer;
};

#endif // BACKEND_H
