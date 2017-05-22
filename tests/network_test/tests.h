#ifndef TESTS_H
#define TESTS_H

#include <QtCore/QObject>

#include "../../src/network/network.h"
#include "../../src/ui/dialogs.h"


class ResultHandler : public QObject {
    Q_OBJECT

    TrDialog dialog;
    bool flag = false;

public:
    ResultHandler();
    ~ResultHandler() override;

public slots:
    void onTrLangsReady(const QJsonObject langs);
    void onDictLangsReady(const QJsonArray langs);
    void onTranslateReady(const QJsonObject result);
    void onLookupRedy(const QJsonObject result);

    void onError(QString error_msg);
};


void trConnector(TrWorker* sender, ResultHandler* receiver);
void dictConnector(DictWorker* sender, ResultHandler* receiver);
#endif // TESTS_H
