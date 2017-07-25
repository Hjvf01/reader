#include "tests.h"


SinglePageViewTest::SinglePageViewTest(const QString& name, VerbosityLevel lvl)
    : BaseTest(name, lvl) {}

SinglePageViewTest::~SinglePageViewTest() {
    cout << "SinglePage Destructor call" << endl;
}

void SinglePageViewTest::testSceneRect() { compareRect(); }


void SinglePageViewTest::testScrollingDown() {
    int h = controller->getView()->height();
    int d_h = doc.get()->size().height();
    int loc = 0;
    while(controller->getLocation() + h < d_h) {
        QTest::keyClick(controller->getView(), Qt::Key_Down);
        QVERIFY(loc == controller->getLocation());
        QVERIFY(0 == controller->getCurrentPage());
        loc += 20;
    }
}

void SinglePageViewTest::testScrollingUp(void) {}

void SinglePageViewTest::testTextBoxes() {
    for(auto base_box: doc.get()->page(0)->baseBoundingBoxes()) {
        auto base_rect = new QGraphicsRectItem(base_box);
        controller->getView()->scene()->addItem(base_rect);
    }
    for(auto actual_box: doc.get()->page(0)->actualBoundingBoxes()) {
        auto actual_rect = new QGraphicsRectItem(actual_box);
        actual_rect->setBrush(Qt::NoBrush);
        actual_rect->setPen(QPen(Qt::red));
        controller->getView()->scene()->addItem(actual_rect);
    }
    controller->getView()->scene()->addItem(new QGraphicsRectItem(
        0,
        0,
        doc.get()->page(0)->baseWidth(),
        doc.get()->page(0)->baseHeight()
    ));
}

void SinglePageViewTest::testDoubleClicked() {
    #define SCENE static_cast<DocScene*>(controller->getView()->scene())

    vector<QPoint> points = {
        QPoint(-47, 301),   // вне сцены
        QPoint(132, 356),   // на странице
        QPoint(870, 280),   // вне сцены
        QPoint(350, 98)     // на слове
    };
    int len = controller->getView()->scene()->items().size();

    SCENE->doubleClick(points[0]);
    SCENE->doubleClick(points[1]);
    SCENE->doubleClick(points[2]);
    QVERIFY(len == SCENE->items().size());

    SCENE->doubleClick(points[3]);
    QVERIFY(len + 1 == SCENE->items().size());

    #undef SCENE
}


void SinglePageViewTest::testSearch() {
    pair<QRectF, QString> result = doc.get()->page(0)->findExactOne("This");
    QVERIFY(result.second == QString("This"));

    result = doc.get()->page(0)->findExactOne("serif");
    QVERIFY(result.second == QString());
}
