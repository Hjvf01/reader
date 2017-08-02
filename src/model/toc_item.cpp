#include "models.h"


TocItem::TocItem(const QString& name, const Link &_link) :
        QStandardItem(name), link(_link) {}

TocItem::TocItem(const QString& name, Link *_link) :
        QStandardItem(name), link(*_link), link_ptr(_link) {}

TocItem::~TocItem() {
    delete link_ptr;
}


int TocItem::getPage() const { return link.pageNumber(); }
