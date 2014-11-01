#include "network.h"

#include <QUdpSocket>
#include <QTcpSocket>

#include "settings.h"

Network::Network(QObject *parent)
    : QObject(parent)
    , m_udp_socket(new QUdpSocket(this))
{
    qDebug("Create Network");
}

Network::~Network()
{
    qDebug("Destroy Network");
}

void Network::init()
{
    qDebug("Init Network");
    m_udp_socket->bind(QHostAddress::Any, Settings::I()->setting("backend/hello_listen_port").toUInt());
}
