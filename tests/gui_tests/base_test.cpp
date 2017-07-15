#include "tests.h"


BaseTest::BaseTest(const QString& name, VerbosityLevel lvl) : level(lvl) {
    QUrl path(base + name);
    doc = DocPtr(
        new PDFDocument(path.path(), path.fileName())
    );
    controller = new DocHandler(doc);

    if(level == VerbosityLevel::verbose) {
        controller->getView()->setWindowTitle(path.path());
        controller->getView()->show();
    }
}


void BaseTest::compareRect() {
    cout << "\t\t\t" << doc.get()->getName().toStdString() << endl;
    QRectF doc_rect(0, 0,
        doc.get()->size()->width(), doc.get()->size()->height());
    QVERIFY(
        controller->getView()->sceneRect() ==
        controller->getView()->scene()->sceneRect()
    );
    QVERIFY(controller->getView()->sceneRect() == doc_rect);
    cout << "doc rect: " << QTest::toString(doc_rect) << "\t"
         << "view rect: " << QTest::toString(
            controller->getView()->sceneRect()
         ) << endl;
}

BaseTest::~BaseTest() {
    cout << "Base Destructor" << endl;
    delete controller;
}

double BaseTest::getCurrentLocation(int loc) {
    return loc / ((double)doc->size()->height() / 100);
}
