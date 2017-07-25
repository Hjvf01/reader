#include <iostream>
using std::cout;
using std::end;
#include <cassert>

#include "ui.h"


DocScene::DocScene() : QGraphicsScene() {
    setBackgroundBrush(Qt::darkGray);
}

DocScene::~DocScene() {
    if(select_box != nullptr)
        removeItem(select_box);
    delete select_box;

    for(auto p: select_boxes)
        if(p != nullptr)
            removeItem(p);
     for(auto p: select_boxes)
        delete p;
     select_boxes.clear();
}


void DocScene::setHightLight(QRectF rect) {
    select_box = new QGraphicsRectItem(rect);
    select_box->setBrush(QBrush(QColor(0, 0, 0, 35)));
    addItem(select_box);
}

void DocScene::setHightLights(const vector<QRectF>& rects) {
    for(QRectF rect: rects) {
        auto item = new QGraphicsRectItem(rect);
        item->setBrush(QBrush(QColor(255, 0, 0, 35)));
        addItem(item);
        select_boxes.push_back(item);
    }
}

QGraphicsRectItem* DocScene::getHightlight() const { return select_box; }


void DocScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {
    emit doubleClick(QPointF(
        event->scenePos().x(),
        event->scenePos().y()
    ));
}

void DocScene::eraseHightlight() {
    if(select_box != nullptr)
        removeItem(select_box);
}

void DocScene::eraseHightlights() {
    for(auto box: select_boxes) {
        if(box != nullptr)
            removeItem(box);
        delete box;
    }
    select_boxes.clear();

    assert(select_boxes.size() == 0);
}
