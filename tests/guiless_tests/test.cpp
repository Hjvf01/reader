#include <iostream>
using std::cout;
using std::endl;
#include <cmath>
using std::abs;

#include <QtCore/QDebug>

#include "tests.h"


PDFDocumentTest::PDFDocumentTest() {
    baseDpi = 101;
    length = paths.size();
    for(int i = 0; i < length; i++)
        docs.append(new PDFDocument(paths[i].path(), paths[i].fileName()));
    for(BaseDocument* doc: docs) {
        doc->setDpi(baseDpi, baseDpi);
        doc->init();
    }
}

PDFDocumentTest::~PDFDocumentTest() {
    for(BaseDocument* doc: docs) delete doc;
}

bool PDFDocumentTest::sizesAlmostEqual(QSize f_size, QSize s_size) {
    int delta_h = f_size.height() / 100;
    int delta_w = f_size.width() / 100;
    return (abs(f_size.height() - s_size.height()) < delta_h) &
           (abs(f_size.width() - s_size.width()) < delta_w);
}

void PDFDocumentTest::test() {
    typedef unsigned int Index;

    for(BaseDocument* doc: docs) {
        Index size = doc->amountPages() > 10 ? 10 : doc->amountPages();
        cout << "\t\t\t" << doc->name().toStdString() << endl;
        for(Index i = 0; i < size; i++) {
            QPixmap page_pix = doc->page(i)->render();
            qDebug() << page_pix.size() << "\t"
                     << doc->page(i)->actualSize();
            QVERIFY(sizesAlmostEqual(
                page_pix.size(), doc->page(i)->actualSize()
            ) == true);
        }
    }
}

void PDFDocumentTest::testPath() {
    for(int i = 0; i < length; i++)
        QVERIFY(docs[i]->path() == paths[i].path());
}

void PDFDocumentTest::testName() {
    for(int i = 0; i < length; i++)
        QVERIFY(docs[i]->name() == paths[i].fileName());
}

void PDFDocumentTest::testAmountPages() {
    for(int i = 0; i < length; i++)
        QVERIFY(docs[i]->pages().size() == docs[i]->amountPages());
}

void PDFDocumentTest::testResizing() {
    cout << "\t\t\t***101***" << endl;
    test();

    cout << "\t\t\t***200.0***" << endl;
    for(BaseDocument* doc: docs) doc->setDpi(200.0, 200.0);
    test();

    cout << "\t\t\t***500.0***" << endl;
    for(BaseDocument* doc: docs) doc->setDpi(500.0, 500.0);
    test();
}

void PDFDocumentTest::testBuild() {
    for(BaseDocument* doc: docs) doc->setDpi(150.0, 150.0);
    for(BaseDocument* doc: docs) qDebug() << *(doc->size());
}
