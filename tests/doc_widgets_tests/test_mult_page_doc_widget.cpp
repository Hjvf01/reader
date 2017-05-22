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
