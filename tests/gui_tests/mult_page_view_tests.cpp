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


void MultPageViewTest::testScrollingUp() {
    while(controller->getLocation() > 0) {
        vector<unsigned int> prev_pages = controller->getIndexes();
        QTest::keyClick(VIEW, Qt::Key_Up);
        vector<unsigned int> after_pages = controller->getIndexes();
        if(prev_pages[0] != after_pages[0]) {
            QVERIFY(
                prev_pages[0] == after_pages[1] &&
                prev_pages[1] == after_pages[2]
            );
            cout << controller->getLocation() << "\t"
                 << getCurrentLocation(controller->getLocation()) << endl;
        }
    }
}


void MultPageViewTest::testGoToNext() {}
void MultPageViewTest::testGoToPrev() {}
void MultPageViewTest::testGoTo() {}


void MultPageViewTest::testSearch() {
    QString text_to_search = "программирование";
    pair<QRectF, QString> result =
        DOC_PTR->page(3)->findExactOne(text_to_search);
    QVERIFY(result.second == text_to_search);

    vector<pair<QRectF, QString>> results =
        DOC_PTR->page(3)->findAll(text_to_search);
     qDebug() << results.size();
}
