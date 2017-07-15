#include "tests.h"


BaseDocWidgetTest::BaseDocWidgetTest(const QString &name, VerbosityLevel lvl) :
        QObject() {

    QUrl path(base + name);
    controller = new DocWidgetHandler(path);

    if(lvl == VerbosityLevel::verbose) {
        controller->getWidget()->setWindowTitle(path.path());
        controller->getWidget()->show();
    }
}

BaseDocWidgetTest::~BaseDocWidgetTest() {
    delete controller;
}
