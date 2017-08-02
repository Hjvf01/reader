#include "ui.h"





PageView::PageView(const QPixmap& pix, Index i) : QGraphicsPixmapItem(pix) {
    index = i;
}


PageView::~PageView() {}


Index PageView::getIndex(void) const { return index; }
