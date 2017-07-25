#include "tests.h"



Handler::Handler() : QObject() {}
Handler::~Handler() {}


bool Handler::getLangs() const { return langs; }
bool Handler::getTranslate() const { return translate; }
bool Handler::getLookup() const { return lookup; }


void Handler::onLangsReady(const QMap<QString, QList<QString>>& result) {
    langs = true;
    qDebug() << result;
}


void Handler::onTranslateReady(const QString& result) {
    translate = true;
    qDebug() << result;
    dialog.setTranslate(result);
    if(dialog_shown == false) {
        dialog.show();
        dialog_shown = true;
    }
}


void Handler::onLookupReady(const QString &result) {
    lookup = true;
    qDebug() << result;
    dialog.setLookup(result);
    if(dialog_shown == false) {
        dialog.show();
        dialog_shown = true;
    }
}
