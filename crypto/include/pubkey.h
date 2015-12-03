#ifndef PUBKEY_H
#define PUBKEY_H

#include <QObject>

class PubKey
    : public QObject
{
    Q_OBJECT

public:
    explicit PubKey(QObject *parent = 0, QByteArray pub_key = "", bool compressed = true);
    ~PubKey();

private:
    QByteArray m_pubkey;
    bool       m_compressed;
};

#endif // PUBKEY_H
