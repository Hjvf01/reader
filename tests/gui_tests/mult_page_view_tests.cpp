#include "tests.h"


MultPageViewTest::MultPageViewTest(const QString& name, VerbosityLevel lvl)
    : BaseTest(name, lvl) {}

MultPageViewTest::~MultPageViewTest() {
    cout << "MultPage Destructor call" << endl;
}


void MultPageViewTest::locationLog(int loc) {
    cout << "Current location: "
         << loc << "\t" << getCurrentLocation(loc) << "%." << endl;
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
        if(prev_pages[2] != after_pages[2]) {
            QVERIFY(
                prev_pages[2] == after_pages[1] &&
                prev_pages[1] == after_pages[0]
            );
            locationLog(controller->getLocation());
        }
        loc += 20;
    }

    vector<unsigned int> indexes = controller->getIndexes();
    for(unsigned int i: indexes)
        cout << i << "\t";
    cout << endl;
    QVERIFY(
        indexes[0] == doc.get()->amountPages() - 3 &&
        indexes[1] == doc.get()->amountPages() - 2 &&
        indexes[2] == doc.get()->amountPages() - 1
    );
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
            locationLog(controller->getLocation());
        }
    }

    vector<unsigned int> indexes = controller->getIndexes();
    for(unsigned int i: indexes)
        cout << i << "\t";
    cout << endl;
    QVERIFY(indexes[0] == 0 && indexes[1] == 1 && indexes[2] == 2);
}


void MultPageViewTest::testGoToNext() {
    while(controller->getCurrentPage() != 30) {
        vector<unsigned int> before_indexes = controller->getIndexes();
        controller->goToNext();
        vector<unsigned int> after_indexes = controller->getIndexes();
        if(controller->getCurrentPage() >= 3)
            for(size_t i = 0; i < after_indexes.size(); ++i)
                QVERIFY(before_indexes[i] + 1 == after_indexes[i]);
    }
}


void MultPageViewTest::testGoToPrev() {
    vector<unsigned int> indexes = controller->getIndexes();
    QVERIFY(indexes[0] == 28 && indexes[1] == 29 && indexes[2] == 30);

    while(controller->getCurrentPage() != 3) {
        auto before_indexes = controller->getIndexes();
        controller->goToPrev();
        auto after_indexes = controller->getIndexes();
        if(controller->getCurrentPage() <= 27)
            for(size_t i = 0; i < after_indexes.size(); ++i)
                QVERIFY(before_indexes[i] == after_indexes[i] + 1);
    }
}


void MultPageViewTest::testGoTo() {}


void MultPageViewTest::testSearch() {
    QString text_to_search = "программирование";
    pair<QRectF, QString> result =
        DOC_PTR->page(3)->findExactOne(text_to_search);
    QVERIFY(result.second == text_to_search);
}
