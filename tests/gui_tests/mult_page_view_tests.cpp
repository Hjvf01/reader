#include <cmath>
using std::abs;
#include <functional>
using std::function;

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


void MultPageViewTest::testGoTo() {
    vector<unsigned int> indexes = controller->getIndexes();
    QVERIFY(indexes[0] == 3 && indexes[1] == 4 && indexes[2] == 5);

    controller->goTo(10);
    indexes = controller->getIndexes();
    QVERIFY(indexes[0] == 10 && indexes[1] == 11 && indexes[2] == 12);

    controller->goTo(40);
    indexes = controller->getIndexes();
    QVERIFY(indexes[0] == 40 && indexes[1] == 41 && indexes[2] == 42);

    controller->goTo(0);
    indexes = controller->getIndexes();
    QVERIFY(indexes[0] == 0 && indexes[1] == 1 && indexes[2] == 2);
    controller->goTo(1);
    indexes = controller->getIndexes();
    QVERIFY(indexes[0] == 0 && indexes[1] == 1 && indexes[2] == 2);
    controller->goTo(2);
    indexes = controller->getIndexes();
    QVERIFY(indexes[0] == 0 && indexes[1] == 1 && indexes[2] == 2);

    unsigned int last = doc.get()->amountPages() - 1;
    controller->goTo(last);
    indexes = controller->getIndexes();
    QVERIFY(
        indexes[0] == last - 2 &&
        indexes[1] == last - 1 &&
        indexes[2] == last
    );
    controller->goTo(last - 1);
    indexes = controller->getIndexes();
    QVERIFY(
        indexes[0] == last - 2 &&
        indexes[1] == last - 1 &&
        indexes[2] == last
    );
    controller->goTo(last - 2);
    indexes = controller->getIndexes();
    QVERIFY(
        indexes[0] == last - 2 &&
        indexes[1] == last - 1 &&
        indexes[2] == last
    );

    controller->goTo(0);
    indexes = controller->getIndexes();
    QVERIFY(indexes[0] == 0 && indexes[1] == 1 && indexes[2] == 2);
}


void MultPageViewTest::testSearch() {
    QString text_to_search = "программирование";
    pair<QRectF, QString> result =
        doc.get()->page(3)->findExactOne(text_to_search);
    QVERIFY(result.second == text_to_search);
}


void MultPageViewTest::testResize() {
    #define SCENE_RECT controller->getView()->scene()->sceneRect()

    function<bool(const QRectF&, const QRectF&, double)> almostEqual =
        [](const QRectF& before, const QRectF& after, double coef) -> bool {
        const double EPSILON = 0.01;
        return (
            (abs(after.width() / before.width() - coef) < EPSILON) &&
            (abs(after.height() / before.height() - coef) < EPSILON)
        );
    };

    vector<unsigned int> indexes = controller->getIndexes();
    QVERIFY(indexes[0] == 0 && indexes[1] == 1 && indexes[2] == 2);

    QRectF before_size = controller->getView()->scene()->sceneRect();
    controller->resize(25);
    QVERIFY(almostEqual(before_size, SCENE_RECT, 0.25));

    while(controller->getCurrentPage() != 10) {
        vector<unsigned int> prev_pages = controller->getIndexes();
        QTest::keyClick(controller->getView(), Qt::Key_Down);
        vector<unsigned int> after_pages = controller->getIndexes();
        if(prev_pages[2] != after_pages[2]) {
            QVERIFY(
                prev_pages[2] == after_pages[1] &&
                prev_pages[1] == after_pages[0]
            );
            locationLog(controller->getLocation());
        }
    }

    indexes = controller->getIndexes();
    for(unsigned int i: indexes)
        cout << i << "\t";
    cout << endl;
}
