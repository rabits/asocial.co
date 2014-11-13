TARGET = ascdatabase

QT += sql
QT -= gui
TEMPLATE = lib
CONFIG += staticlib

HEADERS += \
    include/database/sqldatabase.h \
    include/database/nosqldatabase.h

SOURCES += \
    src/sqldatabase.cpp \
    src/nosqldatabase.cpp

include(lib/leveldb.pri)
include(lib/qsqlcipher/qsql_sqlcipher.pri)
