TEMPLATE = app
TARGET = run
INCLUDEPATH += .
QT += core network widgets testlib
CONFIG += c++14

# Input
SOURCES +=\
    ../../src/network/base_web_worker.cpp\
    ../../src/network/yandex_worker.cpp \
    #
    ../../src/network_handlers/yandex_handler.cpp\
    #
    ../../src/ui/tr_dialog.cpp\
    #
    yandex_worker_test.cpp\
    main.cpp\
    handler.cpp

HEADERS +=\
    ../../src/network/network.h\
    ../../src/ui/dialogs.h\
    ../../src/connectors/connectors.h\
    ../../src/network_handlers/network_handlers.h\
    tests.h
