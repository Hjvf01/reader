TEMPLATE = app
TARGET = run
INCLUDEPATH += .
CONFIG += c++14
QT += widgets testlib core gui xml network

INCLUDEPATH  += /usr/include/poppler/qt5
LIBS         += -L/usr/lib -lpoppler-qt5

# Input
SOURCES += main.cpp\
    ../../src/handlers/doc_handler.cpp\
    ../../src/handlers/main_handler.cpp\
    ../../src/handlers/doc_widget_handler.cpp\
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
    test_doc_widget.cpp \
    test_mult_page_doc_widget.cpp

HEADERS += ../../src/handlers/handlers.h\
    ../../src/ui/ui.h\
    ../../src/ui/dialogs.h\
    ../../src/model/models.h\
    ../../src/network/network.h\
    ../../src/connectors/connectors.h\
    tests.h

RESOURCES = ../../src/reader.qrc
