TARGET = backend

QT -= gui
TEMPLATE = lib

SOURCES += \
    src/backend.cpp

HEADERS += \
    include/backend.h

DEPENDENCY_LIBRARIES = \
    settings \
    crypto \
    network \
    database

include(../dependencies.pri)

unix {
    target.path = $${OUT_PWD}/../dist/lib
    INSTALLS += target
}
