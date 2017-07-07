#include "tests.h"


MultPageViewTest::MultPageViewTest(const QString& name) : BaseTest(name) {}

MultPageViewTest::~MultPageViewTest() {
    cout << "MultPage Destructor call" << endl;
}

void MultPageViewTest::testSceneRect() { compareRect(); }


void MultPageViewTest::testScrollingDown() {
    #define DOC_PTR doc.get()
    #define VIEW controller->getView()

    int h = VIEW->height();
    int d_h = DOC_PTR->size()->height();
    int loc = controller->getLocation();

    while(controller->getLocation() + h < d_h) {
        vector<unsigned int> prev_pages = controller->getIndexes();
        QTest::keyClick(VIEW, Qt::Key_Down);
        QVERIFY(loc == controller->getLocation());
        vector<unsigned int> after_pages = controller->getIndexes();
        if(prev_pages[2] != after_pages[2])
            cout << "current location: "
                 << getCurrentLocation(loc) << " %" << endl;
        loc += 20;
    }
}


void MultPageViewTest::testTextBoxes() {
    auto len = DOC_PTR->amountPages();
    for(unsigned int i = 0; i < len; i++) {
        for(auto base_box: DOC_PTR->page(i)->baseBoundingBoxes()) {
            auto base_rect = new QGraphicsRectItem(base_box);
            VIEW->scene()->addItem(base_rect);
        }
        for(QRectF actual_box: DOC_PTR->page(i)->actualBoundingBoxes()) {
            auto actual_rect = new QGraphicsRectItem(actual_box);
            actual_rect->setBrush(Qt::NoBrush);
            actual_rect->setPen(QPen(Qt::red));
            VIEW->scene()->addItem(actual_rect);
        }
        VIEW->scene()->addItem(new QGraphicsRectItem(
            DOC_PTR->page(i)->topX(), DOC_PTR->page(i)->topY(),
            DOC_PTR->page(i)->baseWidth(), DOC_PTR->page(i)->baseHeight()
        ));
    }
}


void MultPageViewTest::testSearch() {
    QString text_to_search = "программирование";
    pair<QRectF, QString> result =
        DOC_PTR->page(3)->findExactOne(text_to_search);
    QVERIFY(result.second == text_to_search);

    vector<pair<QRectF, QString>> results =
        DOC_PTR->page(3)->findAll(text_to_search);
     qDebug() << results.size();
}
