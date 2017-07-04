#include "ui.h"


PageView::PageView(QPixmap pix, size_t i) : QGraphicsPixmapItem(pix) {
    index = i;
}
PageView::~PageView() {}


size_t PageView::getIndex(void) const { return index; }
