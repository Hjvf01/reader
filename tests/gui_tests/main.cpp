#include <memory>
using std::shared_ptr;

#include <QApplication>
#include <QStyleFactory>

#include "tests.h"


int main(int argc, char** argv) {
    QApplication app(argc, argv);
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    using TestClass = shared_ptr<QObject>;
    using Tests = vector<TestClass>;

    VerbosityLevel lvl = VerbosityLevel::silent;

    Tests tests = {
        TestClass(new SceneTest(lvl)),
        TestClass(new SinglePageViewTest("/single_page.pdf", lvl)),
        TestClass(new MultPageViewTest("/med_doc.pdf", lvl)),
    };
    for(TestClass test: tests)
        QTest::qExec(test.get(), QStringList());

    int res;
    if(lvl == VerbosityLevel::silent)
        res = 0;
    else
        res = app.exec();

    return res;
}
