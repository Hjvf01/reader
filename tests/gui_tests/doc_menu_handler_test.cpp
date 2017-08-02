#include "tests.h"


DocMenuHandlerTest::DocMenuHandlerTest() : QObject() {
    window = new QMainWindow;

    QUrl path(BASE + "/med_doc.pdf");
    doc = DocPtr(new PDFDocument(path.path(), path.fileName()));
    handler = new DocMenuHandler(path);
    window->addDockWidget(Qt::LeftDockWidgetArea, handler->getDocumentMenu());
    window->show();
}

DocMenuHandlerTest::~DocMenuHandlerTest() {
    delete handler;
}


void DocMenuHandlerTest::testTOCItemActivated() {}
