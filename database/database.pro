TARGET = database

QT -= gui
TEMPLATE = lib

SOURCES += \
    src/database.cpp

HEADERS += \
    include/database.h

unix {
    target.path = $${OUT_PWD}/../dist/lib
    INSTALLS += target
}
