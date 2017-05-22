QT += testlib xml core widgets

TEMPLATE = app
TARGET = run
INCLUDEPATH += .

CONFIG += c++14
QMAKE_CXX += -O0

INCLUDEPATH  += /usr/include/poppler/qt5
LIBS         += -L/usr/lib -lpoppler-qt5

SOURCES += main.cpp\
    ../../src/model/base_document.cpp\
    ../../src/model/pdf_document.cpp\
    ../../src/model/pages.cpp \
    test.cpp

HEADERS += ../../src/model/models.h \
    tests.h
