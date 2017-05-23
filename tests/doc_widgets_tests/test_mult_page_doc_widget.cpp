#include <exception>
using std::exception;

#include "tests.h"


TestMultPagesDocWidget::TestMultPagesDocWidget() : BaseTest() {
    path = new QUrl(base + "/med_doc.pdf");
    widget = new DocWidget;
    doc = new PDFDocument(path->path(), path->fileName());
    doc_ctrl = new DocHandler(widget->getView(), doc);
    widget_ctrl = new DocWidgetHandler(widget, doc_ctrl);

    docViewConnector(widget->getView(), doc_ctrl);

    widget->show();
}

TestMultPagesDocWidget::~TestMultPagesDocWidget() {}


void TestMultPagesDocWidget::testResize(void) {
    widget_ctrl->onAbsoluteScaleChanged("110");

    using Index = unsigned int;

    Index len = doc->amountPages();
    QVERIFY(doc_ctrl->getCurrentPage() == 0);
    for(Index i = 0; i < len; i++) {
        if(i == 0 || i == 1 || i == 2)
            QVERIFY(doc->page(i)->isDrawn() == true);
        else
            QVERIFY(doc->page(i)->isDrawn() == false);
    }
}


void TestMultPagesDocWidget::testGoTo() {
    using Index = unsigned int;

    doc_ctrl->goTo(0);
    QVERIFY(doc_ctrl->getCurrentPage() == 0);
    for(Index i = 0; i < doc->amountPages(); i++) {
        if(i == 0 || i == 1 || i == 2)
            QVERIFY(doc->page(i)->isDrawn() == true);
        else
            QVERIFY(doc->page(i)->isDrawn() == false);
    }

    doc_ctrl->goTo(10);
    QVERIFY(doc_ctrl->getCurrentPage() == 10);
    for(Index i = 0; i < doc->amountPages(); i++) {
        if(i == 10 || i == 11 || i == 12)
            QVERIFY(doc->page(i)->isDrawn() == true);
        else
            QVERIFY(doc->page(i)->isDrawn() == false);
    }

    doc_ctrl->goTo(95);
    QVERIFY(doc_ctrl->getCurrentPage() == 95);
    for(Index i = 0; i < doc->amountPages(); i++) {
        if(i == 95 || i == 96 || i == 97)
            QVERIFY(doc->page(i)->isDrawn() == true);
        else
            QVERIFY(doc->page(i)->isDrawn() == false);
    }
    Index last = doc->amountPages() - 1;
    doc_ctrl->goTo(last);
    QVERIFY(doc_ctrl->getCurrentPage() == last);
    for(Index i = 0; i < doc->amountPages(); i++) {
        if(i == last - 2 || i == last - 1 || i == last)
            QVERIFY(doc->page(i)->isDrawn() == true);
        else
            QVERIFY(doc->page(i)->isDrawn() == false);
    }
    Index n = 0;
    doc_ctrl->goTo(n);
    QVERIFY(doc_ctrl->getCurrentPage() == 0);
    for(Index i = 0; i < doc->amountPages(); i++) {
        if(i == 0 || i == 1 || i == 2)
            QVERIFY(doc->page(i)->isDrawn() == true);
        else
            QVERIFY(doc->page(i)->isDrawn() == false);
    }
}
