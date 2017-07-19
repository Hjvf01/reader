#include "models.h"


TocModel::TocModel(QDomDocument* dom, const QString &name)
        : QStandardItemModel() {
    setHorizontalHeaderLabels(QStringList(name));
    if(dom != nullptr) {
        QStandardItem* model_root = invisibleRootItem();
        QDomNode root = dom->firstChild();
        build(root, model_root);
    }
}


void TocModel::build(QDomNode root, QStandardItem *model) {
    while(! root.nextSibling().isNull()) {
        QDomElement elem = root.toElement();
        TocItem* new_item = new TocItem();
    }
}
