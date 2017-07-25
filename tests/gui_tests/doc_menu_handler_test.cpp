#include "tests.h"


DocMenuHandlerTest::DocMenuHandlerTest() : QObject() {
    window = new QMainWindow;

    QUrl path(base + "/med_doc.pdf");
    doc = DocPtr(new PDFDocument(path.path(), path.fileName()));
    handler = new DocMenuHandler(doc);
    window->addDockWidget(Qt::LeftDockWidgetArea, handler->getDocumentMenu());
    window->show();
}

DocMenuHandlerTest::~DocMenuHandlerTest() {
    delete handler;
}


void DocMenuHandlerTest::testTOCItemActivated() {}
