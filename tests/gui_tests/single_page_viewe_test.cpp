#include "tests.h"


SinglePageViewTest::SinglePageViewTest(const QString& name) : BaseTest(name) {}

SinglePageViewTest::~SinglePageViewTest() {
    cout << "SinglePage Destructor call" << endl;
}

void SinglePageViewTest::testSceneRect() { compareRect(); }

void SinglePageViewTest::testScrollingDown() {
    int h = view->height();
    int d_h = doc->size()->height();
    int loc = 0;
    while(controller->getLocation() + h < d_h) {
        QTest::keyClick(view, Qt::Key_Down);
        QVERIFY(loc == controller->getLocation());
        QVERIFY(0 == controller->getCurrentPage());
        loc += 20;
    }
}

void SinglePageViewTest::testScrollingUp(void) {}

void SinglePageViewTest::testTextBoxes() {
    for(auto base_box: doc->page(0)->baseBoundingBoxes()) {
        auto base_rect = new QGraphicsRectItem(base_box);
        view->getScene()->addItem(base_rect);
    }
    for(auto actual_box: doc->page(0)->actualBoundingBoxes()) {
        auto actual_rect = new QGraphicsRectItem(actual_box);
        actual_rect->setBrush(Qt::NoBrush);
        actual_rect->setPen(QPen(Qt::red));
        view->getScene()->addItem(actual_rect);
    }
    view->getScene()->addItem(new QGraphicsRectItem(
        0,
        0,
        doc->page(0)->baseWidth(),
        doc->page(0)->baseHeight()
    ));
}

void SinglePageViewTest::testDoubleClicked() {
    vector<QPoint> points = {
        QPoint(-47, 301),   // вне сцены
        QPoint(132, 356),   // на странице
        QPoint(870, 280),   // вне сцены
        QPoint(350, 98)     // на слове
    };
    int len = view->getScene()->items().size();

    view->getScene()->doubleClick(points[0]);
    view->getScene()->doubleClick(points[1]);
    view->getScene()->doubleClick(points[2]);
    QVERIFY(len == view->getScene()->items().size());

    view->getScene()->doubleClick(points[3]);
    QVERIFY(len + 1 == view->getScene()->items().size());
}


void SinglePageViewTest::testSearch() {
    pair<QRectF, QString> result = doc->page(0)->findExactOne("This");
    QVERIFY(result.second == QString("This"));

    result = doc->page(0)->findExactOne("serif");
    QVERIFY(result.second == QString());
}
