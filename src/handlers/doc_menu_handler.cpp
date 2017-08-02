#include "handlers.h"


DocMenuHandler::DocMenuHandler(const QUrl& path) : QObject() {
    document = DocPtr(new PDFDocument(path.path(), path.fileName()));

    minimap_h = new MinimapHandler(document);
    ui = new DocumentMenu(minimap_h->getMinimap());
    toc_model = new TocModel(
        static_cast<PDFDocument*>(document.get())->getDocument(),
        path.fileName()
    );

    ui->getTocView()->setModel(toc_model);
    initConnectors();
}

DocMenuHandler::~DocMenuHandler() {
    delete toc_model;
    delete minimap_h;
}


void DocMenuHandler::initConnectors() {
    connect(
        ui->getTocView(), &QTreeView::activated,
        this, &DocMenuHandler::onActivated
    );
}


void DocMenuHandler::onActivated(const QModelIndex &index) {
    TocItem* item = dynamic_cast<TocItem*>(
        toc_model->itemFromIndex(index)
    );
    if(item != nullptr)
        emit changePage(QString::number(item->getPage()));
}
