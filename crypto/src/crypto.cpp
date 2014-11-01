#include "crypto.h"

Crypto::Crypto(QObject *parent)
    : QObject(parent)
{
    qDebug("Create Crypto");
}

Crypto::~Crypto()
{
    qDebug("Destroy Crypto");
}

