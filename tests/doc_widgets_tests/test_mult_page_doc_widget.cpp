#include <exception>
using std::exception;

#include "tests.h"


TestMultPagesDocWidget::TestMultPagesDocWidget(QString n) : BaseTest(n) {}
TestMultPagesDocWidget::~TestMultPagesDocWidget() {}


void TestMultPagesDocWidget::pagesAreDrawn(Index fst, Index scd,
                                           Index thrd, Index i) {
    if(i == fst || i == scd || i == thrd)
        QVERIFY(doc->page(i)->isDrawn() == true);
    else
        QVERIFY(doc->page(i)->isDrawn() == false);
}


void TestMultPagesDocWidget::testGoTo() {
    doc_ctrl->goTo(0);
    QVERIFY(doc_ctrl->getCurrentPage() == 0);
    for(Index i = 0; i < doc->amountPages(); i++)
        pagesAreDrawn(0, 1, 2, i);

    doc_ctrl->goTo(10);
    QVERIFY(doc_ctrl->getCurrentPage() == 10);
    for(Index i = 0; i < doc->amountPages(); i++)
        pagesAreDrawn(10, 11, 12, i);

    doc_ctrl->goTo(95);
    QVERIFY(doc_ctrl->getCurrentPage() == 95);
    for(Index i = 0; i < doc->amountPages(); i++)
        pagesAreDrawn(95, 96, 97, i);

    Index last = doc->amountPages() - 1;
    doc_ctrl->goTo(last);
    QVERIFY(doc_ctrl->getCurrentPage() == last);
    for(Index i = 0; i < doc->amountPages(); i++)
        pagesAreDrawn(last - 2, last - 1, last, i);

    Index n = 0;
    doc_ctrl->goTo(n);
    QVERIFY(doc_ctrl->getCurrentPage() == 0);
    for(Index i = 0; i < doc->amountPages(); i++)
        pagesAreDrawn(0, 1, 2, i);
}


void TestMultPagesDocWidget::testOnLastPage() {
    widget_ctrl->onLastPage();
    Index last = doc->amountPages() - 1;
    QVERIFY(doc_ctrl->getCurrentPage() == last);
    Index len = doc->amountPages();
    for(Index i = 0; i < len; i++)
        pagesAreDrawn(last - 2, last - 1, last, i);
}


void TestMultPagesDocWidget::testOnFirstPage() {
    widget_ctrl->onFirstPage();
    QVERIFY(doc_ctrl->getCurrentPage() == 0);
    for(Index i = 0; i < doc->amountPages(); i++)
        pagesAreDrawn(0, 1, 2, i);
}


void TestMultPagesDocWidget::testOnNextPage() {
    widget_ctrl->onFirstPage();

    widget_ctrl->onNextPage();
    QVERIFY(doc_ctrl->getCurrentPage() == 1);
    Index len = doc->amountPages();
    for(Index i = 0; i < len; i++)
        pagesAreDrawn(1, 2, 3, i);

    widget_ctrl->onNextPage();
    QVERIFY(doc_ctrl->getCurrentPage() == 2);
    for(Index i = 0; i < len; i++)
        pagesAreDrawn(2, 3, 4, i);
}


void TestMultPagesDocWidget::testResize() {
    function<double(QRectF, QRectF)> actualDiff =
        [](QRectF f_size, QRectF s_size) -> double {
            if(s_size.height() > f_size.height())
                return (double)(s_size.height() / f_size.height());
            return (double)(f_size.height() / s_size.height());
        };
    function<bool(double, double)> almostEqual =
        [](double actual_factor, double expected_factor) -> bool {
            return fabs(actual_factor - expected_factor) < 0.2;
        };

    QRectF base_size = widget->getView()->getScene()->sceneRect();
    widget_ctrl->onZoomIn();

    QVERIFY(almostEqual(actualDiff(
        widget->getView()->getScene()->sceneRect(), base_size
    ), 1.1));

    widget_ctrl->onZoomIn(); widget_ctrl->onZoomIn();
    QVERIFY(almostEqual(actualDiff(
        widget->getView()->getScene()->sceneRect(), base_size
    ), 1.3));
}


void TestMultPagesDocWidget::testOnPrevPage() {
    widget_ctrl->onLastPage();
    unsigned int len = doc->amountPages();
    for(unsigned int i = 0; i < len; i++)
        pagesAreDrawn(len - 3, len - 2, len - 1, i);

    doc_ctrl->goTo(90);

    QVERIFY(doc_ctrl->getCurrentPage() == 90);
    widget_ctrl->onPrevPage();
    for(unsigned int i = 0; i < len; i++)
        pagesAreDrawn(89, 90, 91, i);
    widget_ctrl->onPrevPage();
    for(unsigned int i = 0; i < len; i++)
        pagesAreDrawn(88, 89, 90, i);

    widget_ctrl->onFirstPage();
    widget_ctrl->onPrevPage();
    for(unsigned int i = 0; i < len; i++)
        pagesAreDrawn(0, 1, 2, i);
}
