#include "crypto/privkey.h"

#include <QDebug>
#include <QDataStream>

PrivKey::PrivKey(QObject *parent, QByteArray privkey, bool encrypted, PubKey *pubkey)
    : QObject(parent)
    , m_privkey(privkey)
    , m_encrypted(encrypted)
    , m_pubkey(pubkey)
{
    qDebug("Create PrivKey");
}

PrivKey::~PrivKey()
{
    m_privkey.fill('*');
    qDebug("Destroy PrivKey");
}

void PrivKey::setPubKey(PubKey *pubkey)
{
    m_pubkey = pubkey;
}

QByteArray PrivKey::getData()
{
    return m_privkey;
}
