#include <memory>
using std::shared_ptr;

#include <QApplication>
#include <QStyleFactory>

#include "tests.h"


int main(int argc, char** argv) {
    QApplication app(argc, argv);
    QApplication::setStyle(QStyleFactory::create("Fusion"));

    VerbosityLevel lvl = VerbosityLevel::verbose;

    vector<QObject*> tests = {
        new SceneTest(lvl),
        new SinglePageViewTest("/single_page.pdf", lvl),
        new MultPageViewTest("/med_doc.pdf", lvl),
        new SingleDocWidgetTest("/single_page.pdf", lvl)
    };
    for(QObject* test: tests)
        QTest::qExec(test, QStringList());

    int res;
    if(lvl == VerbosityLevel::silent)
        res = 0;
    else
        res = app.exec();

    for(QObject* test: tests) delete test;

    return res;
}
