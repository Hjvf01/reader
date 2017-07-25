#include "handlers.h"


DocMenuHandler::DocMenuHandler(const DocPtr& document) : QObject() {
    ui = new DocumentMenu;
    toc_model = new TocModel(
        static_cast<PDFDocument*>(document.get())->getDocument(),
        document.get()->getName()
    );

    ui->getTocView()->setModel(toc_model);
    initConnectors();
}

DocMenuHandler::~DocMenuHandler() {}


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
