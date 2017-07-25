#include "tests.h"


YandexWorkerTest::YandexWorkerTest() : QObject() {
    connector.set(&worker, &handler);
    connector.connect<const QJsonDocument&>(
        vector<void (YandexWorker::*)(const QJsonDocument&)>{
            &YandexWorker::translateReady,
            &YandexWorker::lookupReady,
            &YandexWorker::getLangsReady,
        },
        vector<void (YandexHandler::*)(const QJsonDocument&)>{
            &YandexHandler::translateHandle,
            &YandexHandler::lookupHandle,
            &YandexHandler::langsHandle,
        }
    );

    mock_connector.set(&handler, &mock_handler);
    mock_connector.connect<const QString&>(
        vector<void (YandexHandler::*)(const QString&)>{
            &YandexHandler::translateRedy,
            &YandexHandler::lookupReady,
        },
        vector<void (Handler::*)(const QString&)>{
            &Handler::onTranslateReady,
            &Handler::onLookupReady,
        }
    );
    connect(
        &handler, &YandexHandler::langsReady,
        &mock_handler, &Handler::onLangsReady
    );
}

YandexWorkerTest::~YandexWorkerTest() {}


void YandexWorkerTest::testGetLangs() {
    worker.onGetLangs();
}


void YandexWorkerTest::testTranslate() {
    Parameters params = {
        Parameter("lang", "en-ru"), Parameter("text", text)
    };
    worker.onTranslate(params);
}


void YandexWorkerTest::testLookup() {
    Parameters params = {
        Parameter("lang", "en-ru"), Parameter("text", text),
        Parameter("ui", "ru"),
    };
    worker.onLookup(params);
}
