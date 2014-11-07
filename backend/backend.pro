TARGET = ascbackend

QT -= gui
TEMPLATE = lib
CONFIG += staticlib

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
