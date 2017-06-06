#include "tests.h"


TestSinglePageDocWidget::TestSinglePageDocWidget(QString n) : BaseTest(n) {}
TestSinglePageDocWidget::~TestSinglePageDocWidget() {}


void TestSinglePageDocWidget::testResize() {
    widget_ctrl->onZoomIn();
    QVERIFY(
        widget_ctrl->getWidget()->getToolBar()->getScaleBox()->currentText() ==
        QString("110")
    );
}
