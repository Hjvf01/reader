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
        this, &YandexWorker::onFinished
    );
}

YandexWorker::~YandexWorker() {}


void YandexWorker::setTranslatorParameters(const Parameters &params) {
    for(Parameter param: tr_params.queryItems())
        if(param.first != QString("key"))
            tr_params.removeQueryItem(param.first);

    for(Parameter param: params)
        tr_params.addQueryItem(param.first, param.second);
}


void YandexWorker::setDictParameters(const Parameters &params) {
    for(Parameter param: dict_params.queryItems())
        if(param.first != QString("key"))
            dict_params.removeQueryItem(param.first);

    for(Parameter param: params)
        dict_params.addQueryItem(param.first, param.second);
}


void YandexWorker::onFinished(QNetworkReply *reply) {
    if(replyHasError(reply))
        return;

    QJsonParseError error;
    QJsonDocument response =
        QJsonDocument::fromJson(reply->readAll(), &error);

    if(jsonHasError(error))
        return;

    if(response.isObject())
        emit lookupReady(response);
    else
        emit getLangsReady(response);

    reply->close();
    delete reply;
}


void YandexWorker::onGetLangs() {
    /*
            Метод вернет список языков поддерживаемых Яндекс словарем
            key - API-key
    */
    QUrl url(DICT_URL + "/getLangs");
    url.setQuery(dict_params.query());
    request.setUrl(url);
    manager.get(request);
}


void YandexWorker::onLookup(const Parameters &params) {
    /*
            Метод вернет список частей речи которые может принимать перевод
            целевого языка. Также список примеров перевода.
        key - API-key
        lang - from-to
        text - text to translate
        ui* - user lang
        flags* - FAMILY     = 0x0001 |
                 SHORT_POS  = 0x0002 |
                 MORPHO     = 0x0004 |
                 POS_FILTER = 0x0008 |
    */
    setDictParameters(params);
    QUrl url(DICT_URL + "/lookup");
    url.setQuery(dict_params.query());
    request.setUrl(url);
    manager.get(request);
}


void YandexWorker::onTranslate(const Parameters& params) {
    /*
            Метод вернет перевод слова/выражения с исходного
            языка на целевой
        key - API-key(from db)
        text - text to translate(from arg)
        lang - from-to
        format* - plain | html
    */
    setTranslatorParameters(params);
    QUrl url(TR_URL + "/translate");
    url.setQuery(tr_params.query());
    request.setUrl(url);
    manager.get(request);
}
