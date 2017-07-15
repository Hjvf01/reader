#include <cassert>

#include <QMessageBox>
#include <QFileDialog>
#include <QApplication>

#include "handlers.h"

MainHandler::MainHandler(MainWindow *window) : QObject() {
    ui = window;

    initConnectors();
}

MainHandler::~MainHandler() {
    for(auto d: widgets)      delete d;
    for(auto d: doc_handlers) delete d;
    for(auto d: handlers)     delete d;
    for(auto d: documents)    delete d;
}


void MainHandler::open(const QList<QUrl>& files) {
    for(QUrl path: files) {
        createDocWidget(path);
        assert(
            documents.size() == amount &&
            handlers.size() == amount &&
            widgets.size() == amount
        );
        assert(
            handlers[amount - 1]->getWidget() == widgets[amount - 1] &&
                true
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
    DocPtr doc = DocPtr(new PDFDocument(path.path(), path.fileName()));
    DocHandler* doc_handler = new DocHandler(
        doc
    );
    /*
    DocWidgetHandler* handler = new DocWidgetHandler(
        widget, doc_handler
    );*/

    widgets.push_back(widget);
    //documents.push_back(doc);
    doc_handlers.push_back(doc_handler);
    //handlers.push_back(handler);

    ui->getCentral()->addTab(widget, doc->getName());
    amount++;
}


const vector<void (MainHandler::*)(bool)> MainHandler::fileMenuSlots() const {
    return {
        &MainHandler::onOpen,
        &MainHandler::onPrint,
        &MainHandler::onProperty,
        &MainHandler::onClose,
        &MainHandler::onQuit,
    };
}

const vector<void (MainHandler::*)(bool)> MainHandler::viewMenuSlots() const {
    return {
        &MainHandler::onZoomIn,
        &MainHandler::onZoomOut,
        &MainHandler::onFirstPage,
        &MainHandler::onNextPage,
        &MainHandler::onPrevPage,
        &MainHandler::onLastPage,
        &MainHandler::onFullScreen,
    };
}

const vector<void (MainHandler::*)(bool)> MainHandler::toolMenuSlots() const {
    return {
        &MainHandler::onHighlight,
        &MainHandler::onUnderline,
        &MainHandler::onDashed,
        &MainHandler::onTranslator,
    };
}

const vector<void (MainHandler::*)(bool)> MainHandler::helpMenuSlots() const {
    return {
        &MainHandler::onHelp,
        &MainHandler::onAbout,
    };
}

const vector<void (MainHandler::*)(int)> MainHandler::centralSlots() const {
    return {
        &MainHandler::onTabClicked,
        &MainHandler::onTabChanged,
        &MainHandler::onTabClosed,
    };
}

void MainHandler::initConnectors() {
    file_menu.set(ui->getMenu()->getFileGroup(), this);
    file_menu.connect<bool>(
        ui->getMenu()->getFileGroupSignals(), fileMenuSlots()
    );

    view_menu.set(ui->getMenu()->getViewGroup(), this);
    view_menu.connect<bool>(
        ui->getMenu()->getViewGroupSignals(), viewMenuSlots()
    );

    tools_menu.set(ui->getMenu()->getToolGroup(), this);
    tools_menu.connect<bool>(
        ui->getMenu()->getToolGroupSignals(), toolMenuSlots()
    );

    help_menu.set(ui->getMenu()->getHelpGroup(), this);
    help_menu.connect<bool>(
        ui->getMenu()->getHelpGroupSignals(), helpMenuSlots()
    );

    central.set(ui->getCentral(), this);
    central.connect<int>(
        ui->getCentralSignals(), centralSlots()
    );
}

                /* slots */
void MainHandler::onOpen(bool) {
    ui->statusBarMessage("open");
    QList<QUrl> file_urls = QFileDialog::getOpenFileUrls(
        ui, "Open Document", QUrl("/home"), "*.pdf"
    );
    open(file_urls);
}

void MainHandler::onPrint(bool) {
    ui->statusBarMessage("print");
}

void MainHandler::onProperty(bool) {
    ui->statusBarMessage("property");
    QString title = "Document Information";
    int index = ui->getCentral()->currentIndex();
    if(index == -1)
        QMessageBox::information(ui, title, "");
    else
        QMessageBox::information(ui, title, documents[index]->metaInfo());
}

void MainHandler::onClose(bool) {
    ui->statusBarMessage("close");
    int index = ui->getCentral()->currentIndex();
    if(index == -1)
        return;
    else
        close(index);
}

void MainHandler::onQuit(bool) {
    ui->statusBarMessage("quit");
    qApp->quit();
}

void MainHandler::onZoomIn(bool) {
    ui->statusBarMessage("zoomint in");
    int index = ui->getCentral()->currentIndex();
    if(index == -1)
        return;
    else
        doc_handlers[index]->resize(110);
}

void MainHandler::onZoomOut(bool) {
    ui->statusBarMessage("zooming out");
    int index = ui->getCentral()->currentIndex();
    if(index == -1)
        return;
     else
        doc_handlers[index]->resize(90);
}

void MainHandler::onNextPage(bool) {
    ui->statusBarMessage("on next page");
}

void MainHandler::onLastPage(bool) {
    ui->statusBarMessage("last page");
}

void MainHandler::onFirstPage(bool) {
    ui->statusBarMessage("on first page");
}

void MainHandler::onPrevPage(bool) {
    ui->statusBarMessage("on prev page");
}

void MainHandler::onFullScreen(bool) {
    ui->statusBarMessage("on full screen");
}

void MainHandler::onHelp(bool) {
    ui->statusBarMessage("help");
    HelpDialog dialog;
    dialog.exec();
}

void MainHandler::onAbout(bool) {
    ui->statusBarMessage("about");
    QMessageBox::information(ui, "About", "v - 0.1");
}

void MainHandler::onHighlight(bool) {
    ui->statusBarMessage("hightlight");
}

void MainHandler::onUnderline(bool) {
    ui->statusBarMessage("underline");
}

void MainHandler::onDashed(bool) {
    ui->statusBarMessage("dashed");
}

void MainHandler::onTranslator(bool) {
    ui->statusBarMessage("translator");
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
