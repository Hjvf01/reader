#ifndef TESTS_H
#define TESTS_H


#include <vector>
using std::vector;

#include <QtCore/QObject>
#include <QtTest/QTest>

#include "../../src/ui/dialogs.h"
#include "../../src/network/network.h"
#include "../../src/connectors/connectors.h"
#include "../../src/network_handlers/network_handlers.h"


class Handler : public QObject {
    Q_OBJECT

    bool langs = false;
    bool translate = false;
    bool lookup = false;
    bool dialog_shown = false;

    TrDialog dialog;

public:
    Handler();
    ~Handler() override;

    bool getLangs(void) const;
    bool getTranslate(void) const;
    bool getLookup(void) const;

public slots:
    void onLangsReady(const QMap<QString, QList<QString>>& results);
    void onTranslateReady(const QString& result);
    void onLookupReady(const QString& result);
};


class YandexWorkerTest : public QObject {
    Q_OBJECT

    QString text = "fly";

    YandexWorker worker;
    YandexHandler handler;
    Handler mock_handler;

    One2One<YandexWorker, YandexHandler> connector;
    One2One<YandexHandler, Handler> mock_connector;

public:
    YandexWorkerTest();
    ~YandexWorkerTest() override;

private slots:
    void testGetLangs(void);
    void testTranslate(void);
    void testLookup(void);
};
#endif // TESTS_H
