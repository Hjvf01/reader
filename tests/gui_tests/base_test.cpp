#include "tests.h"


void BaseTest::compareRect() {
    cout << "\t\t\t" << doc->name().toStdString() << endl;
    QRectF doc_rect(0, 0,
        doc->size()->width(), doc->size()->height());
    QVERIFY(view->sceneRect() == view->getScene()->sceneRect());
    QVERIFY(view->sceneRect() == doc_rect);
    cout << "doc rect: " << QTest::toString(doc_rect) << "\t"
         << "view rect: " << QTest::toString(view->sceneRect()) << endl;
}

BaseTest::~BaseTest() {
    cout << "Base Destructor" << endl;
    delete view;
    delete doc;
    delete controller;
    delete path;
}

double BaseTest::getCurrentLocation(int loc) {
    return loc / ((double)doc->size()->height() / 100);
}
