#include <QtCore/Qt>

#include "ui.h"


DocView::DocView(QWidget *parent) : QGraphicsView(parent) {
    scene = new DocScene;
    scroll = new ScrollBar;
    setVerticalScrollBar(scroll);
    scroll_step = 20;
}

DocView::~DocView() {
    delete scene;
}


void DocView::initScene(QSize* size) {
    scene->setSceneRect(0, 0, size->width(), size->height());
    setScene(scene);
}


DocScene* DocView::getScene() const { return scene; }
QScrollBar* DocView::getScroll() const { return scroll; }


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
