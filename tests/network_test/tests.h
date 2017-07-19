#ifndef TESTS_H
#define TESTS_H


#include <memory>
using std::shared_ptr;
#include <vector>
using std::vector;

#include <QtCore/QObject>
#include <QtTest/QTest>

#include "../../src/network/network.h"
#include "../../src/connectors/connectors.h"


class Handler : public QObject {
    Q_OBJECT

    bool langs = false;
    bool translate = false;
    bool lookup = false;

public:
    Handler();
    ~Handler() override;

    bool getLangs(void) const;
    bool getTranslate(void) const;
    bool getLookup(void) const;

public slots:
    void onGetLangs(const QJsonDocument& result);
    void onTranslate(const QJsonDocument& result);
    void onLookup(const QJsonDocument& result);
};


class YandexWorkerTest : public QObject {
    Q_OBJECT

    using Signals =
        BaseConnector<YandexWorker, Handler>::_Signals<const QJsonDocument&>;
    using Slots =
        BaseConnector<YandexWorker, Handler>::_Slots<const QJsonDocument&>;

    YandexWorker worker;
    Handler handler;
    One2One<YandexWorker, Handler> connector;

public:
    YandexWorkerTest();
    ~YandexWorkerTest() override;

private slots:
    void testGetLangs(void);
    void testTranslate(void);
    void testLookup(void);
};
#endif // TESTS_H
