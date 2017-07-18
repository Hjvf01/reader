#include "network.h"


const QString YandexWorker::TR_KEY = "trnsl.1.1.20170510T212921Z"
    ".548f553bcaead33f.e006a894fb485b094ae287188089d6a460d61ae5";
const QString YandexWorker::DICT_KEY = "dict.1.1.20170512T161717Z"
    ".ad55f59bb8ee8774.1c3bd8fb768f39144b375917853fddb2c4203382";

const QString YandexWorker::TR_URL =
    "https://translate.yandex.net/api/v1.5/tr.json";
const QString YandexWorker::DICT_URL =
    "https://dictionary.yandex.net/api/v1/dicservice.json";


YandexWorker::YandexWorker() {
    dict_params.addQueryItem("key", DICT_KEY);
    tr_params.addQueryItem("key", TR_KEY);

    QObject::connect(
        &manager, &QNetworkAccessManager::finished,
        this,, &YandexWorker::onFinished
    );
}

YandexWorker::~YandexWorker() {}


void YandexWorker::onFinished(QNetworkReply *reply) {
    if(replyHasError(reply))
        return;

    QJsonParseError error;
    QJsonDocument response = QJsonDocument::fromJson(reply->readAll(), &error);

    if(jsonHasError(error))
        return;

    if(response.isObject())
        emit lookupReady(response);
    else
        emit getLangsReady(response);

    reply->close();
    delete reply;
}
