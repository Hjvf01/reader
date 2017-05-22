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
    using Tests = vector<Test>;

    Tests tests = {
        Test(new TestSinglePageDocWidget),
        Test(new TestMultPagesDocWidget),
    };

    for(Test test: tests) QTest::qExec(test.get(), QStringList());

    QUrl path(
        "/home/roma/ws/C/scrs/e_reader/tests/samples"
        "/1.The Fellowship of the Ring.pdf"
    );
    PDFDocument doc(path.path(), path.fileName());
    doc.setDpi(120.0, 120.0);
    doc.init();

    QStandardItemModel* model = doc.getToc();
    QTreeView view;
    view.setModel(model);
    view.show();

    return app.exec();
}
