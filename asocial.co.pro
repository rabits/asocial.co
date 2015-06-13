TEMPLATE = subdirs
SUBDIRS += \
    frontend \
    backend \
    settings \
    crypto \
    network \
    database

OTHER_FILES += \
    .qmake.conf \
    dependencies.pri

frontend.depends = \
    settings \
    crypto \
    backend \
    database

backend.depends = \
    settings \
    crypto \
    network \
    database

android {
    SUBDIRS += openssl
    frontend.depends += openssl
    crypto.depends += openssl
    database.depends += openssl
}
