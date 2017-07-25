#include "tests.h"


CentralWidgetTest::CentralWidgetTest() : QObject() {
    handler.initConnectors();
}

CentralWidgetTest::~CentralWidgetTest() {}


void CentralWidgetTest::testOpen() {
    handler.open(paths);
    QTabWidget* central = handler.getWindow()->getCentral();
    QVERIFY(central->count() == paths.size());
}


void CentralWidgetTest::testClose() {
    size_t len = paths.size();
    for(size_t i = 0; i < len; i++)
        handler.close(i);
    QTabWidget* central = handler.getWindow()->getCentral();
    QVERIFY(central->count() == 0);
}
