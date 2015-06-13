TARGET = asocial.bin

QT += qml quick sql
TEMPLATE = app

# Default rules for deployment
include(deployment.pri)

SOURCES += \
    src/main.cpp \
    src/frontend.cpp \
    src/fedatabase.cpp \
    src/accountdatabase.cpp \
    src/storage/internalimageprovider.cpp \
    src/storage/internalstorage.cpp \
    src/wdate.cpp

RESOURCES += \
    qml.qrc

HEADERS += \
    include/frontend/frontend.h \
    src/fedatabase.h \
    src/accountdatabase.h \
    src/storage/internalimageprovider.h \
    src/storage/internalstorage.h \
    src/wdate.h

DEPENDENCY_LIBRARIES = \
    backend \
    settings \
    crypto \
    database \
    network

include(../dependencies.pri)

DISTFILES += \
    android/AndroidManifest.xml \
    android/res/values/libs.xml

android {
    LIBS += ../openssl/libssl.a ../openssl/libcrypto.a
    QT += androidextras

    ANDROID_PACKAGE_SOURCE_DIR = $${OUT_PWD}/android

    android_templates.target = android_templates
    android_templates.commands = rm -rf $$ANDROID_PACKAGE_SOURCE_DIR && cp -a $$PWD/android $$ANDROID_PACKAGE_SOURCE_DIR
    QMAKE_EXTRA_TARGETS += android_templates
    PRE_TARGETDEPS += android_templates
} else {
    CONFIG += link_pkgconfig
    PKGCONFIG += libssl libcrypto
}

# Generate icons
icon_svg = $$PWD/icon.svg
android {
    ldpi.size    = 36x36
    mdpi.size    = 48x48
    hdpi.size    = 72x72
    xhdpi.size   = 96x96
    xxhdpi.size  = 144x144
    xxxhdpi.size = 192x192
    resolutions  = ldpi mdpi hdpi xhdpi xxhdpi xxxhdpi
    for(res, resolutions) {
        path = $$ANDROID_PACKAGE_SOURCE_DIR/res/drawable-$$res
        icon_$${res}.target = icon_$$res
        icon_$${res}.commands = mkdir -p $${path} && ( [ "$${path}/icon.png" -nt "$${icon_svg}" ] || convert -background none -density $$eval($${res}.size) "$${icon_svg}" "$${path}/icon.png" )
        QMAKE_EXTRA_TARGETS += icon_$$res
        POST_TARGETDEPS += icon_$$res
    }
} else {
    icon.target = icon
    icon.commands = [ "icon.png" -nt "$${icon_svg}" ] || convert -background none -density 96 "$${icon_svg}" "icon.png"
    QMAKE_EXTRA_TARGETS += icon
    POST_TARGETDEPS += icon
}
