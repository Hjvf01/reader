#include <memory>
using std::shared_ptr;

#include <QApplication>
#include <QStyleFactory>

#include "tests.h"


int main(int argc, char** argv) {
    QApplication app(argc, argv);
    QApplication::setStyle(QStyleFactory::create("Fusion"));

    using TestPtr = shared_ptr<QObject>;
    using Tests = vector<TestPtr>;

    VerbosityLevel lvl = VerbosityLevel::verbose;

    const Tests tests = {
        TestPtr(new SceneTest(lvl)),
        TestPtr(new SinglePageViewTest("/single_page.pdf", lvl)),
        TestPtr(new MultPageViewTest("/med_doc.pdf", lvl)),
        TestPtr(new SingleDocWidgetTest("/single_page.pdf", lvl)),
        TestPtr(new MultDocWidgetTest("/huge_doc.pdf", lvl)),
    };

    for(TestPtr test: tests)
        QTest::qExec(test.get(), QStringList());

    int res;
    if(lvl == VerbosityLevel::silent)
        res = 0;
    else
        res = app.exec();

    return res;
}
