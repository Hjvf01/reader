#include <iostream>
using std::cout;
using std::end;

#include "ui.h"


DocScene::DocScene() : QGraphicsScene() { setBackgroundBrush(Qt::darkGray); }

DocScene::~DocScene() {
    //removeItem(hightlight);
    //delete hightlight;
}


void DocScene::setHightLight(QRectF rect) {
    hightlight = new QGraphicsRectItem(rect);
    hightlight->setBrush(QBrush(QColor(0, 0, 0, 35)));
    addItem(hightlight);
}

QGraphicsRectItem* DocScene::getHightlight() const { return hightlight; }


void DocScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {
    emit doubleClick(QPointF(
        event->scenePos().x(),
        event->scenePos().y()
    ));
}

void DocScene::eraseHightlight() {
    removeItem(hightlight);
}
