#include "tests.h"


MultPageViewTest::MultPageViewTest() : BaseTest() {
    path = new QUrl(base + "/med_doc.pdf");
    view = new DocView;
    doc = new PDFDocument(path->path(), path->fileName());
    controller = new DocHandler(view, doc);

    docViewConnector(view, controller);
    view->show();
}

MultPageViewTest::~MultPageViewTest() {
    cout << "MultPage Destructor call" << endl;
}

void MultPageViewTest::testSceneRect() { compareRect(); }

void MultPageViewTest::testScrollingDown() {/*
    int h = view->height();
    int d_h = doc->size()->height();
    int loc = controller->getLocation();

    while(controller->getLocation() + h < d_h) {
        QTest::keyClick(view, Qt::Key_Down);
        QVERIFY(loc == controller->getLocation());
        cout << "current location: "
             << getCurrentLocation(loc) << " %" << endl;
        loc += 20;
    }*/
}


void MultPageViewTest::testTextBoxes() {/*
    auto len = doc->amountPages();
    for(unsigned int i = 0; i < len; i++) {
        for(auto base_box: doc->page(i)->baseBoundingBoxes()) {
            auto base_rect = new QGraphicsRectItem(base_box);
            view->getScene()->addItem(base_rect);
        }
        for(QRectF actual_box: doc->page(i)->actualBoundingBoxes()) {
            auto actual_rect = new QGraphicsRectItem(actual_box);
            actual_rect->setBrush(Qt::NoBrush);
            actual_rect->setPen(QPen(Qt::red));
            view->getScene()->addItem(actual_rect);
        }
        view->getScene()->addItem(new QGraphicsRectItem(
            doc->page(i)->topX(), doc->page(i)->topY(),
            doc->page(i)->baseWidth(), doc->page(i)->baseHeight()
        ));
    }*/
}


void MultPageViewTest::testGoTo() {
    using Index = unsigned int;

    controller->goTo(0);
    QVERIFY(controller->getCurrentPage() == 0);
    for(Index i = 0; i < doc->amountPages(); i++) {
        if(i == 0 || i == 1 || i == 2)
            QVERIFY(doc->page(i)->isDrawn() == true);
        else
            QVERIFY(doc->page(i)->isDrawn() == false);
    }

    controller->goTo(10);
    QVERIFY(controller->getCurrentPage() == 10);
    for(Index i = 0; i < doc->amountPages(); i++) {
        if(i == 10 || i == 11 || i == 12)
            QVERIFY(doc->page(i)->isDrawn() == true);
        else
            QVERIFY(doc->page(i)->isDrawn() == false);
    }

    controller->goTo(95);
    QVERIFY(controller->getCurrentPage() == 95);
    for(Index i = 0; i < doc->amountPages(); i++) {
        if(i == 95 || i == 96 || i == 97)
            QVERIFY(doc->page(i)->isDrawn() == true);
        else
            QVERIFY(doc->page(i)->isDrawn() == false);
    }
    Index last = doc->amountPages() - 1;
    controller->goTo(last);
    QVERIFY(controller->getCurrentPage() == last);
    for(Index i = 0; i < doc->amountPages(); i++) {
        if(i == last - 2 || i == last - 1 || i == last)
            QVERIFY(doc->page(i)->isDrawn() == true);
        else
            QVERIFY(doc->page(i)->isDrawn() == false);
    }
}
