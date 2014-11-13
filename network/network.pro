TARGET = ascnetwork

QT -= gui
QT += network
TEMPLATE = lib
CONFIG += staticlib

HEADERS += \
    include/network/network.h \
    include/network/netdevice.h

SOURCES += \
    src/network.cpp \
    src/netdevice.cpp

