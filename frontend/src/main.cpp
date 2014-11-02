#include "frontend.h"

#include <QTime>

#include "settings.h"

void myMessageOutput(QtMsgType type, const QMessageLogContext &, const QString &msg)
{
    switch (type) {
        case QtDebugMsg:
            fprintf(stderr, "[aSocial %s] %s\n", QTime::currentTime().toString("hh:mm:ss.zzz").toLocal8Bit().constData(), msg.toLocal8Bit().constData());
            break;
        case QtWarningMsg:
            fprintf(stderr, "[aSocial] Warning: %s\n", msg.toLocal8Bit().constData());
            break;
        case QtCriticalMsg:
            fprintf(stderr, "[aSocial] Critical: %s\n", msg.toLocal8Bit().constData());
            break;
        case QtFatalMsg:
            fprintf(stderr, "[aSocial] Fatal: %s\n", msg.toLocal8Bit().constData());
            Frontend::destroyI();
            abort();
    }
}

int main(int argc, char *argv[])
{
    qInstallMessageHandler(myMessageOutput);

    QGuiApplication app(argc, argv);

    // Last argument is a configuration file
    if( app.arguments().count() > 1 && QFile::exists(app.arguments().last()) )
        Settings::setConfigFile(app.arguments().last());

    Frontend::I();

    QQmlApplicationEngine engine;

    Frontend::I()->init(&engine, &app);

    qDebug("Loading qml interface");
    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));

    qDebug("application init done, starting");

    int out = app.exec();

    Frontend::destroyI();

    return out;
}
