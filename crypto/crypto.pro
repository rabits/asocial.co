TARGET = asccrypto

QT -= gui
TEMPLATE = lib
CONFIG += staticlib

SOURCES += \
    src/crypto.cpp \
    src/privkey.cpp \
    src/pubkey.cpp

HEADERS += \
    include/crypto.h \
    include/privkey.h \
    include/pubkey.h

unix {
    CONFIG += link_pkgconfig
    PKGCONFIG += libssl libcrypto
}
