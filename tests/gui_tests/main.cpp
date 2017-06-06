#include <QApplication>
#include <QStyleFactory>

#include "tests.h"
#include "../../src/ui/dialogs.h"

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    QApplication::setStyle(QStyleFactory::create("Fusion"));

    using Test = shared_ptr<QObject>;

    vector<Test> tests = {
        Test(new SinglePageViewTest("/single_page.pdf")),
        Test(new MultPageViewTest("/med_doc.pdf")),
    };

    for(Test test: tests) QTest::qExec(test.get(), QStringList());

    FindText dialog;
    dialog.show();

    int res = /*0;*/ app.exec();
    return res;
}
