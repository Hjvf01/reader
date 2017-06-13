#include <iostream>
using std::cout;
using std::endl;

#include <QStyleFactory>
#include <QApplication>
#include <QtCore/QUrl>

#include "handlers/handlers.h"
#include "conectors/connectors.h"

void menuConnector(MainWindow* sender, MainHandler* receiver) {
    MenuBar* m = sender->getMenu();

    QObject::connect(
        m->getOpen(), &QAction::triggered, receiver, &MainHandler::onOpen
    );
    QObject::connect(
        m->getPrint(), &QAction::triggered, receiver, &MainHandler::onPrint
    );
    QObject::connect(
        m->getProperty(), &QAction::triggered,
        receiver, &MainHandler::onProperty
    );
    QObject::connect(
        m->getClose(), &QAction::triggered, receiver, &MainHandler::onClose
    );
    QObject::connect(
        m->getQuit(), &QAction::triggered, qApp, qApp->quit
    );

    QObject::connect(
        m->getZoomIn(), &QAction::triggered, receiver, &MainHandler::onZoomIn
    );
    QObject::connect(
        m->getZoomOut(), &QAction::triggered, receiver, &MainHandler::onZoomOut
    );
    QObject::connect(
        m->getNextPage(), &QAction::triggered,
        receiver, &MainHandler::onNextPage
    );
    QObject::connect(
        m->getPrevPage(), &QAction::triggered,
        receiver, &MainHandler::onPrevPage
    );
    QObject::connect(
        m->getFullScreen(), &QAction::triggered,
        receiver, &MainHandler::onFullScreen
    );

    QObject::connect(
        m->getHightlight(), &QAction::triggered,
        receiver, &MainHandler::onHighlight
    );
    QObject::connect(
        m->getUnderlineText(), &QAction::triggered,
        receiver, &MainHandler::onUnderline
    );
    QObject::connect(
        m->getDashedText(), &QAction::triggered,
        receiver, &MainHandler::onDashed
    );
    QObject::connect(
        m->getTranslator(), &QAction::triggered,
        receiver, &MainHandler::onTranslator
    );

    QObject::connect(
        m->getHelp(), &QAction::triggered, receiver, &MainHandler::onHelp
    );
    QObject::connect(
        m->getAbout(), &QAction::triggered,
        receiver, &MainHandler::onAbout
    );
}

void centralConnector(QTabWidget* sender, MainHandler* receiver) {
    QObject::connect(
        sender, &QTabWidget::currentChanged,
        receiver, &MainHandler::onTabChanged
    );
    QObject::connect(
        sender, &QTabWidget::tabBarClicked,
        receiver, &MainHandler::onTabClicked
    );
    QObject::connect(
        sender, &QTabWidget::tabCloseRequested,
        receiver, &MainHandler::onTabClosed
    );
}


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setStyle(QStyleFactory::create("Fusion"));

    MainWindow main_window;
    MainHandler main_handler(&main_window);

    int res = app.exec();
    qDebug() << res;
    return res;
}
