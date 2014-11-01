#include "backend.h"

#include <QStandardPaths>

#include "settings.h"
#include "network.h"
#include "database.h"

Backend::Backend(QObject *parent)
    : QObject(parent)
    , m_database(NULL)
    , m_network(NULL)
{
    qDebug("Create Backend");

    // UDP ports
    if( Settings::I()->isNull("backend/hello_port") )
        Settings::I()->setting("backend/hello_port", 49100);
    if( Settings::I()->isNull("backend/hello_listen_port") )
        Settings::I()->setting("backend/hello_listen_port", Settings::I()->setting("backend/hello_port").toUInt());

    // Database name & path
    if( Settings::I()->isNull("backend/database_name") )
        Settings::I()->setting("backend/database_name", "public");
    if( Settings::I()->isNull("backend/database_path") )
        Settings::I()->setting("backend/database_path", QStandardPaths::writableLocation(QStandardPaths::DataLocation));
}

Backend::~Backend()
{
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
}
