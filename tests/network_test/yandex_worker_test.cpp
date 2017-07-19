#include "tests.h"


YandexWorkerTest::YandexWorkerTest() : QObject() {
    connector.set(&worker, &handler);
    connector.connect<const QJsonDocument&>(
        Signals{
            &YandexWorker::getLangsReady,
            &YandexWorker::translateReady,
            &YandexWorker::lookupReady
        },
        Slots{
            &Handler::onGetLangs, &Handler::onTranslate, &Handler::onLookup
        }
    );
}

YandexWorkerTest::~YandexWorkerTest() {}


void YandexWorkerTest::testGetLangs() {
    worker.onGetLangs();
    //QVERIFY(handler.getLangs());
}


void YandexWorkerTest::testTranslate() {
    Parameters params = {
        Parameter("lang", "en-ru"), Parameter("text", "a dog")
    };
    worker.onTranslate(params);
}


void YandexWorkerTest::testLookup() {
    Parameters params = {
        Parameter("lang", "en-ru"), Parameter("text", "fly"),
        Parameter("ui", "ru"),
    };
    worker.onLookup(params);
}
