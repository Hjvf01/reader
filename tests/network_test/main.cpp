#include <QtCore/QCoreApplication>
#include <QtWidgets/QApplication>

#include "tests.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    using Parametr = QPair<QString, QString>;

    QString text = "fly";
    TrWorker tr;
    ResultHandler h;
    trConnector(&tr, &h);
    tr.onTranslate(text, {
        Parametr("lang", "en-ru")
    });

    DictWorker dict;
    dictConnector(&dict, &h);
    dict.onLookup(text, {
        Parametr("lang", "en-ru"),
        Parametr("ui", "ru"),
    });
    dict.setParams({Parametr("ui", "ru")});

    int i = app.exec();
    return i;
}
