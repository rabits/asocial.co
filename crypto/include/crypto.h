#ifndef CRYPTO_H
#define CRYPTO_H

#include <QObject>

class Crypto
    : public QObject
{
    Q_OBJECT

public:
    explicit Crypto(QObject *parent = 0);
    ~Crypto();
};

#endif // CRYPTO_H
