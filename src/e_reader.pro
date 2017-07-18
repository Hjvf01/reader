QT       += core gui widgets xml network

TARGET = e_reader
TEMPLATE = app

CONFIG += c++1z

INCLUDEPATH  += /usr/include/poppler/qt5
LIBS         += -L/usr/lib -lpoppler-qt5

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
    model/base_document.cpp \
    model/pdf_document.cpp \
    ui/main_window.cpp \
    ui/menu_bar.cpp \
    ui/doc_view.cpp \
    ui/doc_scene.cpp \
    handlers/main_handler.cpp \
    handlers/doc_handler.cpp \
    ui/doc_tool_bar.cpp \
    ui/doc_widget.cpp \
    handlers/doc_widget_handler.cpp \
    model/page.cpp \
    ui/scroll_bar.cpp \
    ui/page_view.cpp \
    handlers/scene_handler.cpp \
    network/yandex_worker.cpp \
    network/base_web_worker.cpp

HEADERS  += \
    ui/ui.h \
    handlers/handlers.h \
    model/models.h \
    ui/dialogs.h \
    network/network.h \
    connectors/connectors.h

RESOURCES = reader.qrc
