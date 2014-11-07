#include "frontend.h"

#include <signal.h>
#include <QQmlApplicationEngine>
#include <QScreen>
#include <QDir>
#include <QStandardPaths>

#include "settings.h"
#include "backend.h"
#include "database.h"
#include "crypto.h"

Frontend* Frontend::s_pInstance = NULL;

Frontend::Frontend(QObject *parent)
    : QObject(parent)
    , m_translator()
    , m_app(NULL)
    , m_database(NULL)
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
    m_database = new Database(Settings::I()->setting("frontend/database_name").toString(),
                              Settings::I()->setting("frontend/database_path").toString(), this);
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
    Crypto::I()->genKey();
    QByteArray tmp;
    QString orig("1C2dVScJsf35MXLWoEvMaC54MRARd32mrd");
    Crypto::base58DecodeCheck(orig, tmp);
    qDebug() << "Decrypt:" << (tmp == QByteArray::fromHex("0078f8429d34453fce418f0b64f745876d2de25330")) << tmp.toHex();
    QString b58str = Crypto::base58EncodeCheck(tmp);
    qDebug() << "Encrypt:" << (b58str == orig) << b58str;
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
