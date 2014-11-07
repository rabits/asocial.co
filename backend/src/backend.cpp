#include "backend.h"

#include <QStandardPaths>
#include <QTimer>

#include "settings.h"
#include "network.h"
#include "database.h"

Backend::Backend(QObject *parent)
    : QObject(parent)
    , m_database(NULL)
    , m_network(NULL)
    , m_broadcast_timer(NULL)
{
    qDebug("Create Backend");

    // Database name & path
    Settings::I()->setDefault("backend/database_name", "public.asc");
    Settings::I()->setDefault("backend/database_path", QStandardPaths::writableLocation(QStandardPaths::DataLocation));
    // In passive mode backend don't send broadcast messages
    Settings::I()->setDefault("backend/broadcast_passive_mode", false);
    // Interval of sending broadcast requests
    Settings::I()->setDefault("backend/broadcast_interval", 60000);
}

Backend::~Backend()
{
    delete m_broadcast_timer;
    delete m_network;
    delete m_database;
    qDebug("Destroy Backend");
}

void Backend::init()
{
    qDebug("Init Backend");
    m_database = new Database(Settings::I()->setting("backend/database_name").toString(),
                              Settings::I()->setting("backend/database_path").toString(), this);
    m_network = new Network(this);
    m_network->init();

    if( ! Settings::I()->setting("backend/broadcast_passive_mode").toBool() ) {
        qDebug("Starting broadcast active mode");
        m_broadcast_timer = new QTimer(this);
        connect(m_broadcast_timer, SIGNAL(timeout()), SLOT(broadcast()));
        broadcast();
        m_broadcast_timer->start(Settings::I()->setting("backend/broadcast_interval").toULongLong());
    }
}

void Backend::broadcast()
{
    m_network->sendBroadcast(QString("Hello from me"), Network::Hello);
}
