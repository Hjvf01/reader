#include "tests.h"


MultDocWidgetTest::MultDocWidgetTest(const QString &name, VerbosityLevel lvl) :
    BaseDocWidgetTest(name, lvl) {}

MultDocWidgetTest::~MultDocWidgetTest() {}


bool MultDocWidgetTest::compareIndexes(
        const Indexes &actual, const Indexes &expected
) const {
    assert(actual.size() == expected.size());
    Size len = actual.size();
    for(Indx i = 0; i < len; i++)
        if(actual[i] != expected[i])
            return false;
    return true;
}


void MultDocWidgetTest::testFirstPage() {
    #define ACT(I) controller->getToolBar()->getActions()[I]
    #define INDXS  controller->getHandler()->getIndexes()

    QAction* fst_page = ACT(3);

    /* any effect when current page is first */
    Indexes before_indexes = INDXS;
    fst_page->triggered();
    Indexes after_indexes = INDXS;
    Size len = before_indexes.size();
    for(Indx i = 0; i < len; ++i)
        QVERIFY(after_indexes[i] == before_indexes[i]);

    controller->getHandler()->goTo(100);
    vector<unsigned int> indexes = controller->getHandler()->getIndexes();
    QVERIFY(indexes[0] == 100 && indexes[1] == 101 && indexes[2] == 102);

    fst_page->triggered();
    indexes = INDXS;
    QVERIFY(indexes[0] == 0 && indexes[1] == 1 && indexes[2] == 2);
}


void MultDocWidgetTest::testPrevPage() {
    QAction* prev_page = ACT(4);

    /* any effect when current page is first */
    Indexes before_indexes = INDXS;
    prev_page->triggered();
    Indexes after_indexes = INDXS;
    Size len = before_indexes.size();
    for(Indx i = 0; i < len; ++i)
        QVERIFY(before_indexes[i] == after_indexes[i]);

    controller->getHandler()->goTo(200);
    Indexes indexes = {200, 201, 202};
    while(controller->getHandler()->getCurrentPage() != 170) {
        prev_page->triggered();
        for(Indx i = 0; i < 3; i++) indexes[i]--;
        QVERIFY(compareIndexes(INDXS, indexes));
    }

    QVERIFY(compareIndexes(INDXS, Indexes{170, 171, 172}));
}


void MultDocWidgetTest::testNextPage() {
    Indexes before_indexes = INDXS;
    QVERIFY(compareIndexes(before_indexes, Indexes{170, 171, 172}));
    controller->getHandler()->goToNext();
    controller->getHandler()->goToNext();
    QVERIFY(compareIndexes(INDXS, Indexes{170, 171, 172}));

    QAction* next_page = ACT(5);

    Indexes indexes = {170, 171, 172};
    while(controller->getHandler()->getCurrentPage() != 200) {
        next_page->triggered();
        for(Indx i = 0; i < 3; i++) indexes[i]++;
        QVERIFY(compareIndexes(INDXS, indexes));
    }

    QVERIFY(compareIndexes(INDXS, Indexes{198, 199, 200}));
}


void MultDocWidgetTest::testLastPage() {
    QVERIFY(compareIndexes(INDXS, Indexes{198, 199, 200}));

    QAction* last_page = ACT(6);

    last_page->triggered();
    QVERIFY(compareIndexes(INDXS, Indexes{
        controller->getDocument()->amountPages() - 3,
        controller->getDocument()->amountPages() - 2,
        controller->getDocument()->amountPages() - 1
    }));

    /* any effect */
    last_page->triggered();
    QVERIFY(compareIndexes(INDXS, Indexes{
        controller->getDocument()->amountPages() - 3,
        controller->getDocument()->amountPages() - 2,
        controller->getDocument()->amountPages() - 1
    }));
}
