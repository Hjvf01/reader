#include <cassert>


#include "models.h"


TocModel::TocModel() : QStandardItemModel() {}


TocModel::TocModel(const PDFDocPtr& doc, const QString &name)
        : QStandardItemModel(), document(doc) {

    setHorizontalHeaderLabels(QStringList(name));
    _toc = document.get()->toc();
    init(_toc);
}

TocModel::~TocModel() {
    int row_count = rowCount();
    for(int i = 0; i < row_count; i++) {
        QList<QStandardItem*> _list = takeRow(i);
        for(QStandardItem* item: _list) {
            TocModel* _item = dynamic_cast<TocModel*>(item);
            delete _item;
        }
    }
    delete _toc;
}


void TocModel::set(const PDFDocPtr& doc, const QString &name) {
    document = doc;
    setHorizontalHeaderLabels(QStringList(name));
    _toc = document.get()->toc();
    init(_toc);
}


void TocModel::init(QDomDocument *toc) {
    if(toc != nullptr) {
        QStandardItem* model_root = invisibleRootItem();
        QDomNode root = toc->firstChild();
        build(root, model_root);
    }
}


void TocModel::build(QDomNode root, QStandardItem *model) {
    while(! root.nextSibling().isNull()) {
        QDomElement elem = root.toElement();
        TocItem* new_item = nullptr;
        if(! elem.attribute("Destination").isNull()) {
            new_item =
                new TocItem(
                    elem.tagName(),
                    elem.attributeNode("Destination").value()
                );
        } else {
            Poppler::LinkDestination* link = document->linkDestination(
                elem.attributeNode("DestinationName").value()
            );
            new_item = new TocItem(elem.tagName(), link);
        }
        model->appendRow(new_item);
        new_item->setEditable(false);

        if(! root.firstChild().isNull()) {
            build(root.firstChild(), new_item);
        }
        root = root.nextSibling();
    }
}
