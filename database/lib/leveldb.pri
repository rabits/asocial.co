CONFIG(release, debug|release):DEFINES *= NDEBUG
DEFINES += LEVELDB_PLATFORM_POSIX LEVELDB_ATOMIC_PRESENT

android {
    DEFINES += OS_ANDROID
} else {
    DEFINES += OS_LINUX
}

CONFIG += c++11

QMAKE_CFLAGS_WARN_ON -= -Wall
QMAKE_CXXFLAGS_WARN_ON -= -Wall

INCLUDEPATH += $$PWD/leveldb \
    $$PWD/leveldb/include

SOURCES += $$PWD/leveldb/db/builder.cc \
    $$PWD/leveldb/db/c.cc \
    $$PWD/leveldb/db/dbformat.cc \
    $$PWD/leveldb/db/db_impl.cc \
    $$PWD/leveldb/db/db_iter.cc \
    $$PWD/leveldb/db/dumpfile.cc \
    $$PWD/leveldb/db/filename.cc \
    $$PWD/leveldb/db/log_reader.cc \
    $$PWD/leveldb/db/log_writer.cc \
    $$PWD/leveldb/db/memtable.cc \
    $$PWD/leveldb/db/repair.cc \
    $$PWD/leveldb/db/table_cache.cc \
    $$PWD/leveldb/db/version_edit.cc \
    $$PWD/leveldb/db/version_set.cc \
    $$PWD/leveldb/db/write_batch.cc \
    $$PWD/leveldb/table/block_builder.cc \
    $$PWD/leveldb/table/block.cc \
    $$PWD/leveldb/table/filter_block.cc \
    $$PWD/leveldb/table/format.cc \
    $$PWD/leveldb/table/iterator.cc \
    $$PWD/leveldb/table/merger.cc \
    $$PWD/leveldb/table/table_builder.cc \
    $$PWD/leveldb/table/table.cc \
    $$PWD/leveldb/table/two_level_iterator.cc \
    $$PWD/leveldb/util/arena.cc \
    $$PWD/leveldb/util/bloom.cc \
    $$PWD/leveldb/util/cache.cc \
    $$PWD/leveldb/util/coding.cc \
    $$PWD/leveldb/util/comparator.cc \
    $$PWD/leveldb/util/crc32c.cc \
    $$PWD/leveldb/util/env.cc \
    $$PWD/leveldb/util/env_posix.cc \
    $$PWD/leveldb/util/filter_policy.cc \
    $$PWD/leveldb/util/hash.cc \
    $$PWD/leveldb/util/histogram.cc \
    $$PWD/leveldb/util/logging.cc \
    $$PWD/leveldb/util/options.cc \
    $$PWD/leveldb/util/status.cc \
    $$PWD/leveldb/port/port_posix.cc \

TR_EXCLUDE += $$PWD/leveldb/*
