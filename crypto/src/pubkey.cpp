#include "pubkey.h"

#include <QDebug>

PubKey::PubKey(QObject *parent, QByteArray pub_key, bool compressed)
    : QObject(parent)
    , m_pubkey(pub_key)
    , m_compressed(compressed)
{
    qDebug("Create PubKey");
    qDebug() << m_pubkey;
}

PubKey::~PubKey()
{
    qDebug("Destroy PrivKey");
}
