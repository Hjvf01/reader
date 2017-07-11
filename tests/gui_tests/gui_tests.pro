QT += widgets testlib core gui xml network

TEMPLATE = app
TARGET = run
INCLUDEPATH += .
CONFIG += c++14

INCLUDEPATH  += /usr/include/poppler/qt5
LIBS         += -L/usr/lib -lpoppler-qt5

linux-clang {
    message(Linux-clang)
}

linux-g++ {
    message(Linux-g++)
}

SOURCES += main.cpp\
    ../../src/handlers/doc_handler.cpp\
    ../../src/handlers/main_handler.cpp\
    ../../src/handlers/doc_widget_handler.cpp\
    ../../src/handlers/scene_handler.cpp\
    #
    ../../src/ui/doc_scene.cpp\
    ../../src/ui/doc_view.cpp\
    ../../src/ui/main_window.cpp\
    ../../src/ui/menu_bar.cpp\
    ../../src/ui/doc_tool_bar.cpp\
    ../../src/ui/doc_widget.cpp\
    ../../src/ui/scroll_bar.cpp\
    ../../src/ui/page_view.cpp\
    #
    ../../src/model/base_document.cpp\
    ../../src/model/page.cpp\
    ../../src/model/pdf_document.cpp\
    #
    mult_page_view_tests.cpp \
    single_page_viewe_test.cpp \
    base_test.cpp \
    scene_test.cpp

HEADERS +=\
    ../../src/handlers/handlers.h\
    ../../src/ui/dialogs.h\
    ../../src/ui/ui.h\
    ../../src/model/models.h\
    ../../src/network/network.h\
    ../../src/connectors/connectors.h\
    tests.h

CONFIG += debug

QMAKE_CXX += -O1
