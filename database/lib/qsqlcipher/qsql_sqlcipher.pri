HEADERS += $$PWD/qsql_sqlcipher_p.h
SOURCES += $$PWD/qsql_sqlcipher.cpp

INCLUDEPATH += \
    $$QT.core_private.includes \
    $$QT.sql_private.includes

include(sqlcipher.pri)
