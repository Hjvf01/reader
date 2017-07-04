#include <memory>
using std::shared_ptr;
#include <vector>
using std::vector;

#include <QtWidgets/QApplication>
#include <QtWidgets/QTreeView>
#include <QtCore/QStringList>

#include "tests.h"
#include "../../src/model/models.h"


int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    using Test = shared_ptr<QObject>;

    vector<Test> tests = {
        Test(new TestSinglePageDocWidget("/single_page.pdf")),
        Test(new TestMultPagesDocWidget("/med_doc.pdf")),
    };

    for(Test test: tests)
        QTest::qExec(test.get(), QStringList());

    return app.exec();
}
