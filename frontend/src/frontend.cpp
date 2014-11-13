#include "frontend.h"

#include "fedatabase.h"

#include <signal.h>
#include <QQmlApplicationEngine>
#include <QScreen>
#include <QDir>
#include <QStandardPaths>

#include "settings.h"
#include "backend/backend.h"
#include "crypto/crypto.h"

Frontend* Frontend::s_pInstance = NULL;

Frontend::Frontend(QObject *parent)
    : QObject(parent)
    , m_translator()
    , m_app(NULL)
    , m_backend(NULL)
{
    qDebug("Init aSocial v%s", PROJECT_VERSION);

    signal(SIGINT, Frontend::signalHandler);

    QCoreApplication::setOrganizationName("asocial.co");
    QCoreApplication::setOrganizationDomain("asocial.co");
    QCoreApplication::setApplicationName("asocial.co");
    QCoreApplication::setApplicationVersion(PROJECT_VERSION);

    // Application locale
    Settings::I()->setDefault("frontend/locale", QLocale::system().name());
    // Database name & path
    Settings::I()->setDefault("frontend/database_name", "private.asc");
    Settings::I()->setDefault("frontend/database_path", QStandardPaths::writableLocation(QStandardPaths::DataLocation));
}

Frontend::~Frontend()
{
    delete m_backend;
    delete m_database;
    Settings::destroyI();
    qDebug("Destroy Frontend");
}

void Frontend::init(QQmlApplicationEngine *engine, QGuiApplication *app)
{
    qDebug("Init Frontend");

    m_database = new FEDatabase(this, Settings::I()->setting("frontend/database_name").toString(),
                                      Settings::I()->setting("frontend/database_path").toString());
    m_database->open("asdasd");

    m_backend = new Backend(this);
    m_backend->init();

    qDebug("Init context");

    m_app = app;
    m_context = engine->rootContext();

    m_context->setContextProperty("app", this);
    m_context->setContextProperty("settings", Settings::I());
    m_context->setContextProperty("screenScale",
        QGuiApplication::primaryScreen()->physicalDotsPerInch() * QGuiApplication::primaryScreen()->devicePixelRatio() / 100);

    setLocale(Settings::I()->setting("frontend/locale").toString());
    m_app->installTranslator(&m_translator);

    qDebug("Debug: Test gen key");
    PrivKey *key = Crypto::I()->genKey();
    qDebug() << "PrivKey:" << Crypto::base58Encode(key->getData());
    qDebug() << "PrivKey:" << key->getData().toHex();
    qDebug() << "PubKey:" << Crypto::base58Encode(key->getPubKey()->getData());
    qDebug() << "PubKey:" << key->getPubKey()->getData().toHex();
    qDebug() << "Address:" << key->getPubKey()->getAddress();
}

void Frontend::setLocale(QString locale)
{
    qDebug() << "Set locale to" << locale;
    if( ! m_translator.load("tr_" + locale, ":/") )
    {
        m_translator.load("tr_en", ":/");
        Settings::I()->setting("frontend/locale", "en");
    }
}

void Frontend::signalHandler(int signal)
{
    qDebug() << "Received signal:" << signal;
}
