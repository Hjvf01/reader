#include "tests.h"


MultPageViewTest::MultPageViewTest(QString name) : BaseTest(name) {}

MultPageViewTest::~MultPageViewTest() {
    cout << "MultPage Destructor call" << endl;
}

void MultPageViewTest::testSceneRect() { compareRect(); }

void MultPageViewTest::testScrollingDown() {
    int h = view->height();
    int d_h = doc->size()->height();
    int loc = controller->getLocation();

    while(controller->getLocation() + h < d_h) {
        vector<unsigned int> prev_pages = controller->getIndexes();
        QTest::keyClick(view, Qt::Key_Down);
        QVERIFY(loc == controller->getLocation());
        vector<unsigned int> after_pages = controller->getIndexes();
        if(prev_pages[2] != after_pages[2])
            cout << "current location: "
                 << getCurrentLocation(loc) << " %" << endl;
        loc += 20;
    }
}


void MultPageViewTest::testTextBoxes() {
    auto len = doc->amountPages();
    for(unsigned int i = 0; i < len; i++) {
        for(auto base_box: doc->page(i)->baseBoundingBoxes()) {
            auto base_rect = new QGraphicsRectItem(base_box);
            view->getScene()->addItem(base_rect);
        }
        for(QRectF actual_box: doc->page(i)->actualBoundingBoxes()) {
            auto actual_rect = new QGraphicsRectItem(actual_box);
            actual_rect->setBrush(Qt::NoBrush);
            actual_rect->setPen(QPen(Qt::red));
            view->getScene()->addItem(actual_rect);
        }
        view->getScene()->addItem(new QGraphicsRectItem(
            doc->page(i)->topX(), doc->page(i)->topY(),
            doc->page(i)->baseWidth(), doc->page(i)->baseHeight()
        ));
    }
}


void MultPageViewTest::testSearch() {
    QString text_to_search = "программирование";
    pair<QRectF, QString> result =
        doc->page(3)->findExactOne(text_to_search);
    QVERIFY(result.second == text_to_search);

    vector<pair<QRectF, QString>> results =
        doc->page(3)->findAll(text_to_search);
     qDebug() << results.size();
}
