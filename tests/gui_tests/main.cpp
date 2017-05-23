#include <QApplication>
#include <QStyleFactory>

#include "tests.h"

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    QApplication::setStyle(QStyleFactory::create("Fusion"));

    using Test = shared_ptr<QObject>;

    vector<Test> tests = {
        Test(new SinglePageViewTest),
        Test(new MultPageViewTest),
    };

    for(Test test: tests) QTest::qExec(test.get(), QStringList());

    int res = /*0;*/ app.exec();
    return res;
}
