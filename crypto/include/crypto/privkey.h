#ifndef PRIVKEY_H
#define PRIVKEY_H

#include <QObject>

#include "pubkey.h"

class PrivKey
    : public QObject
{
    Q_OBJECT

public:
    explicit PrivKey(QObject *parent = 0, QByteArray privkey = "", bool encrypted = false, PubKey *pubkey = NULL);
    ~PrivKey();

    QByteArray decrypt(QString password);

    void setPubKey(PubKey *pubkey);
    PubKey* getPubKey() { return m_pubkey; }
    QByteArray getData();

private:
    QByteArray m_privkey;
    bool       m_encrypted;
    PubKey    *m_pubkey;
};

#endif // PRIVKEY_H
