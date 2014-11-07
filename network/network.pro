TARGET = ascnetwork

QT -= gui
QT += network
TEMPLATE = lib
CONFIG += staticlib

SOURCES += \
    src/network.cpp \
    src/netdevice.cpp

HEADERS += \
    include/network.h \
    include/netdevice.h

