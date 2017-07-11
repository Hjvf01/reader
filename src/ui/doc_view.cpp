#include <QtCore/Qt>

#include "ui.h"


DocView::DocView(QWidget *parent) : QGraphicsView(parent) {
    scroll = new ScrollBar;
    scroll->setPageStep(scroll_step);
    scroll->setSingleStep(scroll_step);
    setVerticalScrollBar(scroll);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setAlignment(Qt::AlignCenter);
}

DocView::~DocView() {
    delete scroll;
}


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
