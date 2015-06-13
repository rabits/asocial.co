TEMPLATE = lib
CONFIG += staticlib

TARGET = ssl

OTHER_FILES += \
    build.sh

OPENSSL_VERSION = 1.0.2c
OPENSSL_MD5 = 8c8d81a9ae7005276e486702edbcd4b6

openssl_build.target = crypto
openssl_build.commands = $$PWD/build.sh $$OPENSSL_VERSION $$OPENSSL_MD5

PRE_TARGETDEPS += crypto
QMAKE_EXTRA_TARGETS += openssl_build

QMAKE_CC        = true
QMAKE_CXX       = true
QMAKE_LINK      = true
QMAKE_LN_SHLIB  = true
QMAKE_RANLIB    = true
QMAKE_AR        = true
QMAKE_DEL_FILE  = true
