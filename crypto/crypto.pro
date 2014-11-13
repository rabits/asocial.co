TARGET = asccrypto

QT -= gui
TEMPLATE = lib
CONFIG += staticlib

HEADERS += \
    include/crypto/crypto.h \
    include/crypto/privkey.h \
    include/crypto/pubkey.h

SOURCES += \
    src/crypto.cpp \
    src/privkey.cpp \
    src/pubkey.cpp

unix {
    CONFIG += link_pkgconfig
    PKGCONFIG += libssl libcrypto
}
