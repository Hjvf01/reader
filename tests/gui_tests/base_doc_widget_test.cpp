#include "tests.h"


bool almostEqual(const QRectF &before, const QRectF &after, double coef) {
    return (
        (abs(after.width() / before.width() - coef) < EPSILON) &&
        (abs(after.height() / before.height() - coef) < EPSILON)
    );
}


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
