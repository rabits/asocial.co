#!/bin/sh

OPENSSL_VERSION=$1
OPENSSL_MD5=$2

OPENSSL_DIST=openssl-${OPENSSL_VERSION}

[ ! -f libcrypto.a -a ! -f libssl.a ] || exit 0

export ANDROID_DEV=${ANDROID_NDK_ROOT}/platforms/${ANDROID_NDK_PLATFORM}/arch-arm/usr \
    CC=${ANDROID_NDK_ROOT}/toolchains/${ANDROID_NDK_TOOLCHAIN_PREFIX}-${ANDROID_NDK_TOOLCHAIN_VERSION}/prebuilt/${ANDROID_NDK_HOST}/bin/${ANDROID_NDK_TOOLS_PREFIX}-gcc \
    AR=${ANDROID_NDK_ROOT}/toolchains/${ANDROID_NDK_TOOLCHAIN_PREFIX}-${ANDROID_NDK_TOOLCHAIN_VERSION}/prebuilt/${ANDROID_NDK_HOST}/bin/${ANDROID_NDK_TOOLS_PREFIX}-ar

( [ -f ${OPENSSL_DIST}.tar.gz ] || wget https://www.openssl.org/source/${OPENSSL_DIST}.tar.gz ) && \
    echo "${OPENSSL_MD5} ${OPENSSL_DIST}.tar.gz" > ${OPENSSL_DIST}.tar.gz.md5 && \
    md5sum -c ${OPENSSL_DIST}.tar.gz.md5 && \
    tar xf ${OPENSSL_DIST}.tar.gz && \
    cd ${OPENSSL_DIST} && \
    ./Configure android-armv7 no-gost && \
    make build_libs && \
    cp -aL libssl.a libcrypto.a include ../
