#ifndef FRONTEND_H
#define FRONTEND_H

#include <QDebug>

#include <QObject>
#include <QtQml/QQmlContext>
#include <QGuiApplication>
#include <QTranslator>

#include <QtQuick/QQuickView>
#include <qplatformdefs.h>
#include <QQmlApplicationEngine>

class Backend;
class Database;

class Frontend
    : public QObject
{
    Q_OBJECT

public:
    inline static Frontend* I() { if( s_pInstance == NULL ) s_pInstance = new Frontend(); return s_pInstance; }
    inline static void  destroyI() { delete s_pInstance; }
    static void signalHandler(int signal);

    void init(QQmlApplicationEngine *engine, QGuiApplication *app);
    void setLocale(QString locale);

private:
    explicit Frontend(QObject *parent = 0);
    ~Frontend();

    static Frontend *s_pInstance;

    QTranslator      m_translator;
    QGuiApplication *m_app;
    QQmlContext     *m_context;

    Database        *m_database;
    Backend         *m_backend;

public slots:

};

#endif // FRONTEND_H
