#include "crypto/pubkey.h"

#include <QDebug>

#include "crypto/crypto.h"

PubKey::PubKey(QObject *parent, QByteArray pub_key, bool compressed)
    : QObject(parent)
    , m_pubkey(pub_key)
    , m_compressed(compressed)
    , m_address()
{
    qDebug("Create PubKey");
}

PubKey::~PubKey()
{
    qDebug("Destroy PubKey");
}

QString PubKey::getAddress()
{
    if( m_address.isEmpty() ) {
        // Get hashes and prepend 0x00 as main bitcoin network ident and encode with Base58Check
        m_address = Crypto::base58EncodeCheck(Crypto::ripemd160(Crypto::sha256(m_pubkey)).prepend('\0'));
    }

    return m_address;
}

QByteArray PubKey::getData()
{
    return m_pubkey;
}
