#include "frontend/frontend.h"

#include "fedatabase.h"

#include <QDebug>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml/QQmlContext>
#include <QTranslator>
#include <QScreen>
#include <QDir>
#include <QStandardPaths>
#include <QTimer>
#include <QEventLoop>
#include <signal.h>

#include "settings.h"
#include "backend/backend.h"
#include "crypto/crypto.h"

Frontend* Frontend::s_pInstance = NULL;

Frontend::Frontend(QObject *parent)
    : QObject(parent)
    , m_engine(NULL)
    , m_app(NULL)
    , m_translator(NULL)
    , m_device_passkey(NULL)
    , m_backend(NULL)
    , m_database(NULL)
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
    delete m_database;
    delete m_backend;
    delete m_device_passkey;
    Settings::destroyI();
    delete m_translator;
    delete m_engine;
    qDebug("Destroy Frontend");
}

void Frontend::init(QGuiApplication *app)
{
    qDebug("Init Frontend");

    m_app = app;
    connect(app, SIGNAL(aboutToQuit()), this, SLOT(deleteMe()));

    initInterface();
    initLocale();
    initEngine();

    // Run postinit after application starts
    QTimer::singleShot(0, this, SLOT(postinit()));
}

void Frontend::initInterface()
{
    qDebug("Init Frontend interface");

    m_engine = new QQmlApplicationEngine(this);
    m_context = m_engine->rootContext();

    m_context->setContextProperty("app", this);
    m_context->setContextProperty("settings", Settings::I());
    m_context->setContextProperty("screenScale",
                                  QGuiApplication::primaryScreen()->physicalDotsPerInch() * QGuiApplication::primaryScreen()->devicePixelRatio() / 100);
}

void Frontend::initLocale()
{
    qDebug("Init Frontend locale");
    m_translator = new QTranslator(this);
    setLocale(Settings::I()->setting("frontend/locale").toString());
    m_app->installTranslator(m_translator);
}

void Frontend::initEngine()
{
    qDebug("Loading qml interface");
    m_engine->load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
}

void Frontend::postinit()
{
    qDebug("PostInit Frontend");

    postinitDevicePassKey();
    postinitBackend();
    postinitDatabase();
}

void Frontend::postinitDevicePassKey()
{
    qDebug("PostInit Frontend device pass key");

    if( Settings::I()->isNull("common/device_passkey") ) {
        qDebug("Create new device passkey");

        m_device_passkey = Crypto::I()->genKey();

        QString password = passwordGetWait(qtTrId("Encrypt your Device PassKey"), true);

        if( ! password.isEmpty() ) {
            m_device_passkey->encrypt(password);
            password.fill('*');
        } else
            qWarning("Device PassKey is not encrypted");

        Settings::I()->setDefault("common/device_passkey", QString(m_device_passkey->getData().toHex()));
        Settings::I()->setDefault("common/device_passkey_encrypted", m_device_passkey->isEncrypted());
    } else {
        m_device_passkey = new PrivKey(this, QByteArray::fromHex(Settings::I()->setting("common/device_passkey").toString().toLocal8Bit()),
                                             Settings::I()->setting("common/device_passkey_encrypted").toBool());
    }

    while( ! m_device_passkey->decryptForever(passwordGetWait(qtTrId("Enter your Device PassKey password"))) ) {}
}

void Frontend::postinitBackend()
{
    qDebug("PostInit Frontend backend");
    m_backend = new Backend(this);
    m_backend->init(m_device_passkey);
}

void Frontend::postinitDatabase()
{
    qDebug("PostInit Frontend database");
    m_database = new FEDatabase(this, Settings::I()->setting("frontend/database_name").toString(),
                                      Settings::I()->setting("frontend/database_path").toString());
    m_database->open(QString(Crypto::sha3256(m_device_passkey->getData()).toHex()));
}

void Frontend::setLocale(QString locale)
{
    qDebug() << "Set locale to" << locale;
    if( ! m_translator->load("tr_" + locale, ":/") )
    {
        m_translator->load("tr_en", ":/");
        Settings::I()->setting("frontend/locale", "en");
    }
}

QString Frontend::passwordGetWait(const QString &description, const bool create)
{
    qDebug() << "Request password and wait";
    m_password = "";
    QString password;

    emit requestPassword(description, create);

    QEventLoop wait_loop;
    connect(this, &Frontend::donePassword, &wait_loop, &QEventLoop::exit);
    if( wait_loop.exec() == -1 ) {
        qFatal("Frontend closed. Exiting.");
    }

    password = m_password;
    m_password = "";
    return password;
}

void Frontend::responsePassword(const QString password)
{
    m_password = password;
    emit donePassword(0);
}

void Frontend::signalHandler(int signal)
{
    qDebug() << "Received signal:" << signal;
}
