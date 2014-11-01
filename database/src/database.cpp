#include "database.h"

#include <QDebug>

Database::Database(QObject *parent)
    : QObject(parent)
    , m_name()
    , m_path()
{
    qDebug() << "Create Database without name";
}

Database::Database(QString name, QString path, QObject *parent)
    : QObject(parent)
    , m_name(name)
    , m_path(path)
{
    qDebug() << "Create Database" << m_name;
}

Database::~Database()
{
    qDebug() << "Destroy Database" << m_name;
}
