#include "tests.h"



Handler::Handler() : QObject() {}
Handler::~Handler() {}


bool Handler::getLangs() const { return langs; }
bool Handler::getTranslate() const { return translate; }
bool Handler::getLookup() const { return lookup; }


void Handler::onGetLangs(const QJsonDocument &result) {
    langs = true;
    qDebug() << result;
}


void Handler::onTranslate(const QJsonDocument &result) {
    translate = true;
    qDebug() << result;
}


void Handler::onLookup(const QJsonDocument &result) {
    lookup = true;
    qDebug() << result;
}
