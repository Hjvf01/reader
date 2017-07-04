#include <iostream>
using std::cout;
using std::endl;

#include <QStyleFactory>
#include <QApplication>
#include <QtCore/QUrl>

#include "handlers/handlers.h"


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setStyle(QStyleFactory::create("Fusion"));

    MainWindow main_window;
    MainHandler main_handler(&main_window);

    int res = app.exec();
    qDebug() << res;
    return res;
}
