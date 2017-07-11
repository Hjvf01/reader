#include "tests.h"


SceneTest::SceneTest(VerbosityLevel lvl) : QObject(), level(lvl) {
    QUrl path(base + "/single_page.pdf");
    doc = shared_ptr<BaseDocument>(
        new PDFDocument(path.path(), path.fileName())
    );
    controller = new SceneHandler(doc);

    doc.get()->setDpi(101, 101);
    doc.get()->init();
    controller->getScene()->setSceneRect(
        0, 0, doc.get()->size()->width(), doc.get()->size()->height()
    );
    view.setScene(controller->getScene());

    if(level == VerbosityLevel::verbose)
        view.show();
}

SceneTest::~SceneTest() {}


void SceneTest::testDbClick() {
    unsigned int len = doc.get()->amountPages();
    vector<PageView*> page_views(len);
    for(unsigned int i = 0; i < len; i++) {
        PageView* page_v = new PageView(doc.get()->page(i)->render(), i);
        page_v->setOffset(doc.get()->page(i)->offset());
        controller->getScene()->addItem(page_v);
        page_views[i] = page_v;
    }


    vector<QPointF> points = {
        QPointF(164, 406),
        QPointF(330, 94),
    };
    controller->getScene()->doubleClick(points[0]);
    Q_ASSERT((unsigned int)controller->getScene()->items().size() == len);
    controller->getScene()->doubleClick(points[1]);
    Q_ASSERT((unsigned int)controller->getScene()->items().size() == len + 1);
}
