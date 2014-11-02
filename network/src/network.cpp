#include "network.h"

#include <QDebug>
#include <QUdpSocket>
#include <QTcpSocket>

#include "settings.h"

Network::Network(QObject *parent)
    : QObject(parent)
    , m_udp_socket(new QUdpSocket(this))
{
    qDebug("Create Network");

    // broadcast UDP target port
    Settings::I()->setDefault("network/broadcast_port", 49100);
    // broadcast UDP bind host (leave empty to listen both IPv4 & IPv6)
    Settings::I()->setDefault("network/broadcast_listen_host", "");
    // broadcast UDP bind port
    Settings::I()->setDefault("network/broadcast_listen_port", Settings::I()->setting("network/broadcast_port").toUInt());
}

Network::~Network()
{
    delete m_udp_socket;
    qDebug("Destroy Network");
}

void Network::init()
{
    qDebug("Init Network");

    qDebug("Binging broadcast port");
    QString bind_host = Settings::I()->setting("network/broadcast_listen_host").toString();
    quint16 bind_port = Settings::I()->setting("network/broadcast_listen_port").toUInt();
    if( m_udp_socket->bind(bind_host.isEmpty() ? QHostAddress::Any : QHostAddress(bind_host), bind_port) )
        connect(m_udp_socket, SIGNAL(readyRead()), SLOT(readUDP()));
    else
        qCritical() << "I can't hear broadcast messages because unable to bind UDP host:" << bind_host << "port:" << bind_port;
}

void Network::sendUDP(QString str, UDPType type)
{
    qDebug() << "Sending UDP message:" << str;
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);

    out << quint64(0);
    out << quint8(type);
    out << str;
    out.device()->seek(quint64(0));
    out << quint64(data.size() - sizeof(quint64));

    m_udp_socket->writeDatagram(data, QHostAddress::Broadcast, Settings::I()->setting("network/broadcast_port").toUInt());
}

void Network::readUDP()
{
    while (m_udp_socket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(m_udp_socket->pendingDatagramSize());
        QHostAddress sender;
        quint16 sender_port;

        m_udp_socket->readDatagram(datagram.data(), datagram.size(), &sender, &sender_port);

        qDebug() << "Received UDP datagram from" << sender.toString() << sender_port;

        QDataStream in(&datagram, QIODevice::ReadOnly);

        quint64 size = 0;
        if(in.device()->size() > sizeof(quint64))
            in >> size;
        else
            return;
        if (in.device()->size() - sizeof(quint64) < size)
            return;

        quint8 type = 0;
        in >> type;

        if (type == Hello) {
            QString str;
            in >> str;
            qDebug() << "Received UDP message:" << str;
        }
    }
}

