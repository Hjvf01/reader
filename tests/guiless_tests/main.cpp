#include <memory>
using std::unique_ptr;
using std::shared_ptr;

#include <QtWidgets/QApplication>

#include "tests.h"


int main(int argc, char** argv) {
    QApplication a(argc, argv);
    Q_UNUSED(a)
    using TestClass = shared_ptr<QObject>;

    vector<TestClass> tests = {
        TestClass(new PDFDocumentTest)
    };

    for(TestClass test: tests) QTest::qExec(test.get(), QStringList());
    return 0;
}
