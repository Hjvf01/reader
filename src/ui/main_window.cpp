#include "ui.h"


MainWindow::MainWindow() : QMainWindow() {
    menu = new MenuBar(this);
    setMenuBar(menu);

    central_widget = new QTabWidget(this);
    central_widget->setTabsClosable(true);
    setCentralWidget(central_widget);

    status = new QStatusBar(this);
    setStatusBar(status);

    setMinimumSize(640, 480);

    show();
}

MenuBar* MainWindow::getMenu() const { return menu; }
QTabWidget* MainWindow::getCentral() const { return central_widget; }

void MainWindow::statusBarMessage(QString msg) {
    status->showMessage(msg);
}


MainWindow::~MainWindow() {
    delete menu;
    delete central_widget;
    delete status;
}
