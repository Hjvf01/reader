QT       += core gui widgets xml network

TARGET = e_reader
TEMPLATE = app

CONFIG += c++1z

INCLUDEPATH  += /usr/include/poppler/qt5
LIBS         += -L/usr/lib -lpoppler-qt5


DEFINES += QT_DEPRECATED_WARNINGS


SOURCES +=\
    main.cpp\
    #
    ui/main_window.cpp\
    ui/menu_bar.cpp\
    ui/doc_view.cpp\
    ui/doc_scene.cpp\
    ui/scroll_bar.cpp\
    ui/doc_tool_bar.cpp\
    ui/doc_widget.cpp\
    ui/page_view.cpp\
    ui/tr_dialog.cpp\
    ui/document_menu.cpp\
    #
    model/base_document.cpp\
    model/pdf_document.cpp\
    model/page.cpp\
    model/toc_model.cpp\
    model/toc_item.cpp\
    #
    handlers/main_handler.cpp\
    handlers/doc_handler.cpp\
    handlers/doc_widget_handler.cpp\
    handlers/scene_handler.cpp\
    handlers/doc_menu_handler.cpp\
    #
    network/yandex_worker.cpp\
    network/base_web_worker.cpp\
    #
    network_handlers/yandex_handler.cpp \
    ui/find_dialog.cpp \
    ui/help_dialog.cpp


HEADERS  +=\
    ui/ui.h\
    ui/dialogs.h\
    model/models.h\
    handlers/handlers.h\
    network/network.h\
    connectors/connectors.h\
    exceptions/exceptions.h \
    network_handlers/network_handlers.h

RESOURCES = reader.qrc
