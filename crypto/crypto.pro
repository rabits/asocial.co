TARGET = crypto

QT -= gui
TEMPLATE = lib

SOURCES += \
    src/crypto.cpp

HEADERS += \
    include/crypto.h

unix {
    target.path = $${OUT_PWD}/../dist/lib
    INSTALLS += target
}
