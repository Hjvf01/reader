#include "tests.h"


SceneTest::SceneTest() : QObject() {
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
    view.show();
}

SceneTest::~SceneTest() {}


void SceneTest::testDbClick() {
    size_t len = doc.get()->amountPages();
    vector<PageView*> page_views(len);
    for(size_t i = 0; i < len; i++) {
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
    Q_ASSERT(controller->getScene()->items().size() == len);
    controller->getScene()->doubleClick(points[1]);
    Q_ASSERT(controller->getScene()->items().size() == len + 1);
}
