TARGET = network

QT -= gui
QT += network
TEMPLATE = lib

SOURCES += \
    src/network.cpp

HEADERS += \
    include/network.h

unix {
    target.path = $${OUT_PWD}/../dist/lib
    INSTALLS += target
}
