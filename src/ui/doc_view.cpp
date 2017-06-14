#include <QtCore/Qt>

#include "ui.h"


DocView::DocView(QWidget *parent) : QGraphicsView(parent) {
    scene = new DocScene;
    scroll = new ScrollBar;
    scroll->setPageStep(scroll_step);
    scroll->setSingleStep(scroll_step);
    qDebug() << scroll->pageStep();
    setVerticalScrollBar(scroll);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setAlignment(Qt::AlignCenter);
}

DocView::~DocView() {
    delete scene;
    delete scroll;
}


void DocView::initScene(QSize* size) {
    scene->setSceneRect(0, 0, size->width(), size->height());
    setScene(scene);
}


DocScene* DocView::getScene() const { return scene; }
ScrollBar* DocView::getScroll() const { return scroll; }


void DocView::keyPressEvent(QKeyEvent *event) {
    if(event->key() == 16777235)
        emit scrollUp(scroll_step);
    else if(event->key() == 16777237)
        emit scrollDown(scroll_step);
    else
        qDebug() << event->key();
}


void DocView::wheelEvent(QWheelEvent *event) {
    if(event->angleDelta().y() < 0)
        emit scrollDown(scroll_step);
    else
        emit scrollUp(scroll_step);
}


const vector<void (DocView::*)(int)> DocView::getScrollSignals(void) const {
    vector<void (DocView::*)(int)> res = {
        &DocView::scrollUp,
        &DocView::scrollDown,
    };
    return res;
}
