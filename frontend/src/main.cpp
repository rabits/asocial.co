#include "frontend/frontend.h"

#include <QTime>
#include <QGuiApplication>
#include <QFile>
#include <cstdio>

#include "settings.h"

void myMessageOutput(QtMsgType type, const QMessageLogContext &, const QString &msg)
{
    switch (type) {
        case QtDebugMsg:
            ::std::fprintf(stderr, "[aSocial %s] %s\n", QTime::currentTime().toString("hh:mm:ss.zzz").toLocal8Bit().constData(), msg.toLocal8Bit().constData());
            break;
        case QtWarningMsg:
            ::std::fprintf(stderr, "[aSocial] Warning: %s\n", msg.toLocal8Bit().constData());
            break;
        case QtCriticalMsg:
            ::std::fprintf(stderr, "[aSocial] Critical: %s\n", msg.toLocal8Bit().constData());
            break;
        case QtFatalMsg:
            ::std::fprintf(stderr, "[aSocial] Fatal: %s\n", msg.toLocal8Bit().constData());
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

    Frontend::I()->init(&app);

    qDebug("Application init done, starting");

    return app.exec();
}
