#include <cassert>

#include <QMessageBox>
#include <QFileDialog>

#include "handlers.h"

MainHandler::MainHandler(MainWindow *window) : QObject() {
    ui = window;
}

MainHandler::~MainHandler() {
    for(auto d: widgets)      delete d;
    for(auto d: doc_handlers) delete d;
    for(auto d: handlers)     delete d;
    for(auto d: documents)    delete d;
}

void MainHandler::open(QList<QUrl> files) {
    for(QUrl path: files) {
        createDocWidget(path);
        assert(
            documents.size() == amount &&
            handlers.size() == amount &&
            widgets.size() == amount
        );
        assert(
            handlers[amount - 1]->getWidget() == widgets[amount - 1] &&
            doc_handlers[amount - 1]->getDoc() == documents[amount - 1]
        );
    }
}

void MainHandler::close(int index) {
    /*
        1. Выделить удаляемые объекты
        2. Удалить их из коллекций
        3. осводить ресурсы
    */
    ui->getCentral()->removeTab(index);

    BaseDocument* deleted_doc = documents[index];
    DocWidget* deleted_widget = widgets[index];
    DocHandler* deleted_doc_handler = doc_handlers[index];
    DocWidgetHandler* deleted_handler = handlers[index];

    documents.erase(documents.begin() + index);
    widgets.erase(widgets.begin() + index);
    doc_handlers.erase(doc_handlers.begin() + index);
    handlers.erase(handlers.begin() + index);

    assert(
        documents.size() == amount - 1 &&
        handlers.size() == amount - 1 &&
        widgets.size() == amount - 1 &&
        doc_handlers.size() == amount - 1
    );

    delete deleted_doc;
    delete deleted_widget;
    delete deleted_handler;
    delete deleted_doc_handler;

    amount--;
}

void MainHandler::createDocWidget(QUrl path) {
    /*
        1. Создать виджет документа, документ,
           обработчик сигналов представления документа,
           обработчик сигналов виджета.
        2. Добавить их в соответсвующие коллекции.
        3. добавить виджет документа к цетральному виджету приложения
        4. Соединить сигналы предстадения с его обработчиком
    */
    DocWidget* widget = new DocWidget;
    BaseDocument* doc = new PDFDocument(path.path(), path.fileName());
    DocHandler* doc_handler = new DocHandler(
        widget->getView(), doc
    );
    DocWidgetHandler* handler = new DocWidgetHandler(
        widget, doc_handler
    );

    widgets.push_back(widget);
    documents.push_back(doc);
    doc_handlers.push_back(doc_handler);
    handlers.push_back(handler);

    ui->getCentral()->addTab(widget, doc->name());

    docViewConnector(widget->getView(), doc_handler);
    amount++;
}


                /* slots */
void MainHandler::onOpen() {
    ui->statusBarMessage("open");
    QList<QUrl> file_urls = QFileDialog::getOpenFileUrls(
        ui, "Open Document", QUrl("/home"), "*.pdf"
    );
    open(file_urls);
}

void MainHandler::onPrint() {
    ui->statusBarMessage("print");
}

void MainHandler::onProperty() {
    ui->statusBarMessage("property");
    QString title = "Document Information";
    int index = ui->getCentral()->currentIndex();
    if(index == -1)
        QMessageBox::information(ui, title, "");
    else
        QMessageBox::information(ui, title, documents[index]->metaInfo());
}

void MainHandler::onClose() {
    ui->statusBarMessage("close");
    int index = ui->getCentral()->currentIndex();
    if(index == -1)
        return;
    else
        close(index);
}

void MainHandler::onZoomIn() {
    ui->statusBarMessage("zoomint in");
    int index = ui->getCentral()->currentIndex();
    if(index == -1)
        return;
    else
        doc_handlers[index]->rel_resize(0.1);
}

void MainHandler::onZoomOut() {
    ui->statusBarMessage("zooming out");
    int index = ui->getCentral()->currentIndex();
    if(index == -1)
        return;
     else
        doc_handlers[index]->rel_resize(-0.1);
}

void MainHandler::onNextPage() {
    ui->statusBarMessage("on next page");
}

void MainHandler::onPrevPage() {
    ui->statusBarMessage("on prev page");
}

void MainHandler::onFullScreen() {
    ui->statusBarMessage("on full screen");
}

void MainHandler::onHelp() {
    ui->statusBarMessage("help");
    QString msg =
          "File Menu\n"
            "Ctrl+O:\tOpen\n"
            "Ctrl+P:\tPrint\n"
            "Alt+Return:\tDocument Info\n"
            "Ctrl+W:\tClose Document\n"
            "Ctrl+Q:\tQuit\n\n"
          "Info Menu\n"
            "Ctrl+H:\tHelp\n";
   QMessageBox::information(ui, "Help", msg);
}

void MainHandler::onAbout() {
    ui->statusBarMessage("about");
    QMessageBox::information(ui, "About", "v - 0.1");
}


void MainHandler::onTabChanged(int index) {
    QString msg = "Current tab: ";
    msg += QString::number(index);
    ui->statusBarMessage(msg);
}

void MainHandler::onTabClicked(int index) {
    QString msg = "Current tab: " + QString::number(index);
    ui->statusBarMessage(msg);
}

void MainHandler::onTabClosed(int index) {
    ui->statusBarMessage("tab closed");
    close(index);
}
/* end slots */
