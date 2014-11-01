TARGET = settings

QT -= gui
TEMPLATE = lib

SOURCES += \
    src/settings.cpp

HEADERS += \
    include/settings.h

unix {
    target.path = $${OUT_PWD}/../dist/lib
    INSTALLS += target
}
