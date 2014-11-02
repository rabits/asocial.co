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
    enum UDPType {
        Hello
    };

    explicit Network(QObject *parent = 0);
    ~Network();

    void init();

    void sendUDP(QString str, UDPType type);

private slots:
    void readUDP();

private:
    QUdpSocket *m_udp_socket;
};

#endif // NETWORK_H
