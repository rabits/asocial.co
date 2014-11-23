#ifndef FRONTEND_H
#define FRONTEND_H

#include <QObject>

class QGuiApplication;
class QQmlApplicationEngine;
class QTranslator;
class QQmlContext;

class PrivKey;
class Backend;
class FEDatabase;

class Frontend
    : public QObject
{
    Q_OBJECT

public:
    inline static Frontend* I() { if( s_pInstance == NULL ) s_pInstance = new Frontend(); return s_pInstance; }
    inline static void destroyI() { delete s_pInstance; }
    static void signalHandler(int signal);

    void init(QGuiApplication *app);
    Q_INVOKABLE void postinit();

    void setLocale(QString locale);

    QString passwordGetWait(const QString &description, const bool create = false);

signals:
    void requestPassword(const QString description, const bool create);
    void donePassword(int code);

public slots:
    void deleteMe() { Frontend::destroyI(); }
    void responsePassword(const QString password);

private:
    explicit Frontend(QObject *parent = 0);
    ~Frontend();

    static Frontend *s_pInstance;

    void initInterface();
    void initLocale();
    void initEngine();

    void postinitDevicePassKey();
    void postinitBackend();
    void postinitDatabase();

    QQmlApplicationEngine *m_engine;
    QGuiApplication *m_app;
    QQmlContext     *m_context;
    QTranslator     *m_translator;

    QString          m_password;

    PrivKey         *m_device_passkey;
    Backend         *m_backend;
    FEDatabase      *m_database;

};

#endif // FRONTEND_H
