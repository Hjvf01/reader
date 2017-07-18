#include <cassert>

#include <QMessageBox>
#include <QFileDialog>
#include <QApplication>

#include "handlers.h"

MainHandler::MainHandler() : QObject() { ui = new MainWindow; }
MainHandler::~MainHandler() {}


void MainHandler::open(const QList<QUrl>& files) {
    for(QUrl path: files) {
        DocWidgetPtr widget(new DocWidgetHandler(path));
        documents.push_back(widget);
        ui->getCentral()->addTab(
            widget.get()->getWidget(), widget.get()->getDocument()->getName()
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
    documents.erase(documents.begin() + index);
}


void MainHandler::initConnectors() {
    file_menu.set(ui->getMenu()->getFileGroup(), this);
    file_menu.connect<bool>(
        Signals<QAction, bool>{
            ui->getMenu()->fileGroupSize(), &QAction::triggered
        },
        Slots<MainHandler, bool>{
            &MainHandler::onOpen, &MainHandler::onPrint,
            &MainHandler::onProperty, &MainHandler::onClose,
            &MainHandler::onQuit,
        }
    );

    view_menu.set(ui->getMenu()->getViewGroup(), this);
    view_menu.connect<bool>(
        vector<void (QAction::*)(bool)>{
            ui->getMenu()->viewGroupSize(), &QAction::triggered
        },
        vector<void (MainHandler::*)(bool)>{
            &MainHandler::onZoomIn, &MainHandler::onZoomOut,
            &MainHandler::onFirstPage, &MainHandler::onNextPage,
            &MainHandler::onPrevPage, &MainHandler::onLastPage,
            &MainHandler::onFullScreen,
        }
    );

    tools_menu.set(ui->getMenu()->getToolGroup(), this);
    tools_menu.connect<bool>(
        vector<void (QAction::*)(bool)>{
            ui->getMenu()->toolGroupSize(), &QAction::triggered
        },
        vector<void (MainHandler::*)(bool)>{
            &MainHandler::onHighlight, &MainHandler::onUnderline,
            &MainHandler::onDashed, &MainHandler::onTranslator,
        }
    );

    help_menu.set(ui->getMenu()->getHelpGroup(), this);
    help_menu.connect<bool>(
        vector<void (QAction::*)(bool)>{
            ui->getMenu()->helpGroupSize(), &QAction::triggered
        },
        vector<void (MainHandler::*)(bool)>{
            &MainHandler::onHelp, &MainHandler::onAbout,
        }
    );

    central.set(ui->getCentral(), this);
    central.connect<int>(
        vector<void (QTabWidget::*)(int)>{
            &QTabWidget::tabBarClicked, &QTabWidget::currentChanged,
            &QTabWidget::tabCloseRequested,
        },
        vector<void (MainHandler::*)(int)>{
            &MainHandler::onTabClicked, &MainHandler::onTabChanged,
            &MainHandler::onTabClosed,
        }
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
        QMessageBox::information(
            ui, title, documents[index].get()->getDocument()->metaInfo()
        );
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
        documents[index].get()->getHandler()->resize(110);
}

void MainHandler::onZoomOut(bool) {
    ui->statusBarMessage("zooming out");
    int index = ui->getCentral()->currentIndex();
    if(index == -1)
        return;
     else
        documents[index].get()->getHandler()->resize(90);
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
