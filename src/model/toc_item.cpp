#include "models.h"


TocItem::TocItem(const QString& name, const Link &_link)
    : QStandardItem(name), link(_link) {}

TocItem::TocItem(const QString& name, Link *_link)
    : QStandardItem(name), link(*(_link)) {}

TocItem::~TocItem() {}


void TocItem::set(const Link &_link) { link = _link; }
void TocItem::set(Link *_link) { link = *_link; }
