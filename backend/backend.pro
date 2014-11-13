TARGET = ascbackend

QT -= gui
TEMPLATE = lib
CONFIG += staticlib

SOURCES += \
    src/backend.cpp \
    src/bedatabase.cpp

HEADERS += \
    include/backend/backend.h \
    src/bedatabase.h

DEPENDENCY_LIBRARIES = \
    settings \
    crypto \
    network \
    database

include(../dependencies.pri)
