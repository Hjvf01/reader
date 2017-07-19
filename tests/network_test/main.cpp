#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

#include "tests.h"


int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    using Test = shared_ptr<QObject>;
    using Tests = vector<Test>;

    const Tests tests = {
        Test(new YandexWorkerTest),
    };

    for(Test test: tests)
        QTest::qExec(test.get(), QStringList());


    QWidget widget;
    QPushButton button;
    QVBoxLayout layout;
    widget.setFixedSize(150, 50);
    widget.setLayout(&layout);
    layout.addWidget(&button);
    button.setText("OK");
    widget.show();
    QObject::connect(
        &button, &QPushButton::pressed, []() { qApp->quit(); }
    );

    return app.exec();
}
