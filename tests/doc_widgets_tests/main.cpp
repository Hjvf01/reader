#include <memory>
using std::shared_ptr;
#include <vector>
using std::vector;

#include <QtWidgets/QApplication>
#include <QtCore/QStringList>

#include "tests.h"


int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    using Test = shared_ptr<QObject>;
    using Tests = vector<Test>;

    Tests tests = {
        Test(new TestSinglePageDocWidget),
        Test(new TestMultPagesDocWidget),
    };

    for(Test test: tests) QTest::qExec(test.get(), QStringList());

    return app.exec();
}
