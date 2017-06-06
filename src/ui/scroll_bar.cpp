#include "ui.h"


ScrollBar::ScrollBar(QWidget *parent) : QScrollBar(parent) {}
ScrollBar::ScrollBar() : QScrollBar() {}
ScrollBar::~ScrollBar() {}

/*
void ScrollBar::mouseMoveEvent(QMouseEvent *event) {
    qDebug() << event->pos()
             << sliderPosition();
}


void ScrollBar::mousePressEvent(QMouseEvent *event) {
    qDebug() << event->pos()
             << sliderPosition();
}


void ScrollBar::mouseReleaseEvent(QMouseEvent *event) {
    qDebug() << event->pos()
             << sliderPosition();
}
*/
