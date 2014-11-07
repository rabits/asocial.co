TARGET = asocial.bin

QT += qml quick
TEMPLATE = app

# Default rules for deployment
include(deployment.pri)

SOURCES += \
    src/main.cpp \
    src/frontend.cpp

RESOURCES += qml.qrc

HEADERS += \
    src/frontend.h

OTHER_FILES += \
    scripts/asocial

DEPENDENCY_LIBRARIES = \
    settings \
    crypto \
    backend \
    database \
    network

include(../dependencies.pri)

unix {
    CONFIG += link_pkgconfig
    PKGCONFIG += libssl libcrypto

    target.path = $${OUT_PWD}/../dist
    INSTALLS += target

    scripts.path = $${OUT_PWD}/../dist
    scripts.files = scripts/*
    INSTALLS += scripts
}
