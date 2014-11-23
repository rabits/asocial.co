#ifndef PUBKEY_H
#define PUBKEY_H

#include <QObject>

class PubKey
    : public QObject
{
    Q_OBJECT

public:
    explicit PubKey(QObject *parent = 0, const QByteArray &pub_key = "", bool compressed = true);
    ~PubKey();

    const QString getAddress();
    const QByteArray getData() const;

private:
    QByteArray m_pubkey;
    bool       m_compressed;
    QString    m_address;
};

#endif // PUBKEY_H
