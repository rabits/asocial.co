#ifndef NETWORK_H
#define NETWORK_H

#include <QObject>

class QUdpSocket;
class QTcpSocket;

class Network
    : public QObject
{
    Q_OBJECT

public:
    explicit Network(QObject *parent = 0);
    ~Network();

    void init();

private:
    QUdpSocket *m_udp_socket;
};

#endif // NETWORK_H
