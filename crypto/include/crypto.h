#ifndef CRYPTO_H
#define CRYPTO_H

#include <QObject>

#include "pubkey.h"
#include "privkey.h"

typedef struct ec_group_st EC_GROUP;

class Crypto
    : public QObject
{
    Q_OBJECT

public:
    inline static Crypto* I() { if( s_pInstance == NULL ) s_pInstance = new Crypto(); return s_pInstance; }
    inline static void  destroyI() { delete s_pInstance; }

    void init();

    PrivKey* genKey();

    static bool base58Decode(const QString& b58str, QByteArray& data);
    static bool base58DecodeCheck(const QString& b58str, QByteArray& data);
    static QString base58Encode(const QByteArray& data);
    static QString base58EncodeCheck(const QByteArray& data);

    static inline QByteArray sha256enc(const QByteArray &data); // Used to generate address of profile or device
    static inline QByteArray sha3256enc(const QByteArray &data); // Used in messages digest

private:
    explicit Crypto(QObject *parent = 0);
    ~Crypto();
    static Crypto *s_pInstance;

    EC_GROUP *m_group;
};

#endif // CRYPTO_H
