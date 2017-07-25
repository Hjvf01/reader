#include <QApplication>
#include <QPushButton>
#include <QWidget>
#include <QVBoxLayout>
#include <QStyleFactory>
#include <QTreeView>

#include "tests.h"


int main(int argc, char** argv) {
    QApplication app(argc, argv);
    QApplication::setStyle(QStyleFactory::create("Fusion"));

    using TestPtr = shared_ptr<QObject>;
    using Tests = vector<TestPtr>;

    VerbosityLevel lvl = VerbosityLevel::verbose;

    const Tests tests = {
        //TestPtr(new SceneTest(lvl)),
        //TestPtr(new SinglePageViewTest("/single_page.pdf", lvl)),
        //TestPtr(new MultPageViewTest("/med_doc.pdf", lvl)),
        TestPtr(new SingleDocWidgetTest("/single_page.pdf", lvl)),
        TestPtr(new MultDocWidgetTest("/huge_doc.pdf", lvl)),
        TestPtr(new DocMenuHandlerTest),
        TestPtr(new CentralWidgetTest),
    };

    for(TestPtr test: tests)
        QTest::qExec(test.get(), QStringList());

    QUrl path(base + "/huge_doc.pdf");
    auto document = PDFDocPtr(Poppler::Document::load(path.path()));
    TocModel model(document, path.fileName());
    QTreeView view;
    view.setModel(&model);
    view.show();

    QWidget close_widget;
    QVBoxLayout layout;
    QPushButton close;
    close_widget.setLayout(&layout);
    layout.addWidget(&close);
    close.setText("Close");
    QObject::connect(&close, &QPushButton::pressed, [](){ qApp->quit(); });
    close_widget.setFixedSize(150, 50);
    close_widget.show();

    int res = app.exec();

    return res;
}
