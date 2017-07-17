#include "tests.h"


SingleDocWidgetTest::SingleDocWidgetTest(
    const QString &name,
    VerbosityLevel lvl
) : BaseDocWidgetTest(name, lvl) {}

SingleDocWidgetTest::~SingleDocWidgetTest() {}


void SingleDocWidgetTest::testZoomIn() {
    #define SCENE_RECT controller->getHandler()->getView()->sceneRect()
    #define GET_ACT(I) controller->getWidget()->getToolBar()->getActions()[I]

    QAction* zoom_in = GET_ACT(0);

    base_size = SCENE_RECT;
    zoom_in->triggered();
    QRectF after_size = SCENE_RECT;
    QVERIFY(almostEqual(base_size, after_size, 1.1));

    zoom_in->triggered();
    after_size = SCENE_RECT;
    QVERIFY(almostEqual(base_size, after_size, 1.2));

    zoom_in->triggered();
    after_size = SCENE_RECT;
    QVERIFY(almostEqual(base_size, after_size, 1.3));

    zoom_in->triggered();
    after_size = SCENE_RECT;
    QVERIFY(almostEqual(base_size, after_size, 1.4));

    zoom_in->triggered();
    after_size = SCENE_RECT;
    QVERIFY(almostEqual(base_size, after_size, 1.5));

    /* any effect after 150% */
    zoom_in->triggered();
    QVERIFY(after_size == SCENE_RECT);
    zoom_in->triggered();
    QVERIFY(after_size == SCENE_RECT);
}


void SingleDocWidgetTest::testZoomOut() {
    QAction* zoom_out = GET_ACT(1);

    while(SCENE_RECT != base_size)
        zoom_out->triggered();
    QVERIFY(base_size == SCENE_RECT);

    vector<double> coefs = {0.9, 0.8, 0.7, 0.6, 0.5};
    for(double coef: coefs) {
        zoom_out->triggered();
        QVERIFY(almostEqual(base_size, SCENE_RECT, coef));
    }

    /* any effect below 50% */
    QRectF before_size = SCENE_RECT;
    zoom_out->triggered();
    QVERIFY(before_size == SCENE_RECT);
}


void SingleDocWidgetTest::testScaleBox() {
    #define CB(I) controller->getWidget()->getToolBar()->getComboBoxes()[I]

    vector<unsigned int> indexes = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    vector<double> coefs = {
        0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2, 1.3, 1.4, 1.5
    };

    QComboBox* scale_box = CB(0);

    assert(coefs.size() == indexes.size());
    unsigned int len = coefs.size();
    for(unsigned int i = 0; i < len; i++) {
        scale_box->setCurrentIndex(indexes[i]);
        QVERIFY(almostEqual(base_size, SCENE_RECT, coefs[i]));
    }

    scale_box->setCurrentIndex(indexes[0]);

    #undef CB
    #undef GET_ACT
    #undef SCENE_RECT
}
