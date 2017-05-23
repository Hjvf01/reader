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
        qDebug() << i;
        if(i == 0 || i == 1 || i == 2)
            QVERIFY(doc->page(i)->isDrawn() == true);
        else
            QVERIFY(doc->page(i)->isDrawn() == false);
    }
}
