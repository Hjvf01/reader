#include <memory>
using std::shared_ptr;

#include <QApplication>
#include <QStyleFactory>

#include "tests.h"
#include "../../src/ui/dialogs.h"

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    using TestClass = shared_ptr<QObject>;
    using Tests = vector<TestClass>;

    Tests tests = {
        TestClass(new SinglePageViewTest("/single_page.pdf")),
        TestClass(new MultPageViewTest("/med_doc.pdf")),
        //TestClass(new SceneTest()),
    };
    for(TestClass test: tests)
        QTest::qExec(test.get(), QStringList());

    int res = /*0;*/ app.exec();
    return res;
}
