TEMPLATE = lib
CONFIG += staticlib

TARGET = ssl

OTHER_FILES += \
    build.sh

OPENSSL_VERSION = 1.0.2d
OPENSSL_MD5 = 38dd619b2e77cbac69b99f52a053d25a

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
