TARGET = ascdatabase

QT += sql
QT -= gui
TEMPLATE = lib
CONFIG += staticlib

SOURCES += \
    src/sqldatabase.cpp \
    src/nosqldatabase.cpp

HEADERS += \
    include/sqldatabase.h \
    include/nosqldatabase.h

include(lib/leveldb.pri)
include(lib/qsqlcipher/qsql_sqlcipher.pri)
