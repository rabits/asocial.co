#include "frontend.h"

#include <QTime>

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
            abort();
    }
}

int main(int argc, char *argv[])
{
    qInstallMessageHandler(myMessageOutput);

    Frontend::I();

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    Frontend::I()->init(&engine, &app);

    qDebug("Loading qmls");
    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));

    qDebug("Init done, starting");

    int out = app.exec();

    Frontend::destroyI();

    return out;
}
