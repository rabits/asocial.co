TARGET = asocial.bin

QT += qml quick sql
TEMPLATE = app

# Default rules for deployment
include(deployment.pri)

SOURCES += \
    src/main.cpp \
    src/frontend.cpp \
    src/fedatabase.cpp \
    src/accountdatabase.cpp

RESOURCES += qml.qrc

HEADERS += \
    include/frontend/frontend.h \
    src/fedatabase.h \
    src/accountdatabase.h

OTHER_FILES += \
    scripts/asocial

DEPENDENCY_LIBRARIES = \
    backend \
    settings \
    crypto \
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
