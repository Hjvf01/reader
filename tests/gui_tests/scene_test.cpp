#include "tests.h"


SceneTest::SceneTest() : QObject() {
    QUrl path(base + "/single_page.pdf");
    doc = new PDFDocument(path.path(), path.fileName());
    scene = new DocScene;
    controller = new SceneHandler(scene, doc);

    doc->setDpi(101, 101);
    doc->init();
    scene->setSceneRect(0, 0, doc->size()->width(), doc->size()->height());
    view.setScene(scene);
    view.show();
}

SceneTest::~SceneTest() {
    delete controller;
    delete doc;
    delete scene;
}


void SceneTest::testDbClick() {
    size_t len = doc->amountPages();
    vector<PageView*> page_views(len);
    for(size_t i = 0; i < len; i++) {
        PageView* page_v = new PageView(doc->page(i)->render(), i);
        page_v->setOffset(doc->page(i)->offset());
        scene->addItem(page_v);
        page_views[i] = page_v;
    }

    vector<QPointF> points = {
        QPointF(164, 406),
        QPointF(330, 94),
    };
    scene->doubleClick(points[0]);
    Q_ASSERT((unsigned int)scene->items().size() == (unsigned int)len);
    scene->doubleClick(points[1]);
    Q_ASSERT((unsigned int)scene->items().size() == (unsigned int)len + 1);
}
