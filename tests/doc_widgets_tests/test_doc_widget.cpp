#include "tests.h"


TestSinglePageDocWidget::TestSinglePageDocWidget() : BaseTest() {
    path = new QUrl(base + "/single_page.pdf");
    widget = new DocWidget;
    doc = new PDFDocument(path->path(), path->fileName());
    doc_ctrl = new DocHandler(widget->getView(), doc);
    widget_ctrl = new DocWidgetHandler(widget, doc_ctrl);

    docViewConnector(widget->getView(), doc_ctrl);

    widget->show();
}

TestSinglePageDocWidget::~TestSinglePageDocWidget() {}
