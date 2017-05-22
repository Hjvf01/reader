#include "tests.h"


void trConnector(TrWorker *sender, ResultHandler *receiver) {
    QObject::connect(
        sender, &TrWorker::getLangsReady,
        receiver, &ResultHandler::onTrLangsReady
    );
    QObject::connect(
        sender, &TrWorker::translateReady,
        receiver, &ResultHandler::onTranslateReady
    );
    QObject::connect(
        sender, &BaseWebWorker::errorSignal, receiver, &ResultHandler::onError
    );
}

void dictConnector(DictWorker *sender, ResultHandler *receiver) {
    QObject::connect(
        sender, &DictWorker::getLangsReady,
        receiver, &ResultHandler::onDictLangsReady
    );
    QObject::connect(
        sender, &DictWorker::lookupReady,
        receiver, &ResultHandler::onLookupRedy
    );
    QObject::connect(
        sender, &BaseWebWorker::errorSignal, receiver, &ResultHandler::onError
    );
}


ResultHandler::ResultHandler() : QObject() {}

ResultHandler::~ResultHandler() {}


void ResultHandler::onError(QString error_msg) {
    qDebug() << "Error: " << error_msg;
}

void ResultHandler::onTrLangsReady(const QJsonObject langs) {
    qDebug() << "Translator langs: " << langs;
}

void ResultHandler::onDictLangsReady(const QJsonArray langs) {
    qDebug() << "Dict langs: " << langs;
}


void ResultHandler::onTranslateReady(const QJsonObject result) {
    dialog.setTranslate(result);
    if(flag == false) dialog.show();
    flag = true;
}

void ResultHandler::onLookupRedy(const QJsonObject result) {
    dialog.setLookup(result);
    if(flag == false) dialog.show();
    flag = true;
}
