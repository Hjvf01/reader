TEMPLATE = app
TARGET = run
INCLUDEPATH += .
QT += core network widgets testlib
CONFIG += c++14

# Input
SOURCES +=\
    main.cpp\
    ../../src/network/base_web_worker.cpp\
    ../../src/network/yandex_worker.cpp \
    yandex_worker_test.cpp \
    handler.cpp

HEADERS +=\
    ../../src/network/network.h\
    ../../src/ui/dialogs.h \
    ../../src/connectors/connectors.h\
    tests.h
