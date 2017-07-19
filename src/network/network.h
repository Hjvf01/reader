#ifndef NETWORK_H
#define NETWORK_H


#include <vector>
using std::vector;

#include <QtNetwork/QSslSocket>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QSslConfiguration>

#include <QtCore/QUrl>
#include <QtCore/QObject>
#include <QtCore/QUrlQuery>
#include <QtCore/QByteArray>
#include <QtCore/QJsonParseError>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonObject>


using Parameter = QPair<QString, QString>;
using Parameters = QList<Parameter>;

using JsonError = QJsonParseError::ParseError;
using NetworkError = QNetworkReply::NetworkError;


class BaseWebWorker : public QObject {
    Q_OBJECT

protected:
    QNetworkAccessManager manager;
    QNetworkRequest request;
    QUrl url;
    QSslConfiguration config;

public:
    BaseWebWorker();
    ~BaseWebWorker();

signals:
    void errorSignal(const QString& msg) const;

protected:
    bool jsonHasError(const QJsonParseError& error) const;
    bool replyHasError(QNetworkReply* reply) const;
};


class YandexWorker : public BaseWebWorker {
    Q_OBJECT

    static const QString TR_KEY;
    static const QString DICT_KEY;

    static const QString TR_URL;
    static const QString DICT_URL;

    QUrlQuery dict_params;
    QUrlQuery tr_params;
public:
    YandexWorker();
    ~YandexWorker() override;

public slots:
    void onGetLangs(void);
    void onLookup(const Parameters& params);
    void onTranslate(const Parameters& params);

private slots:
    void onFinished(QNetworkReply* reply);

private:
    void setTranslatorParameters(const Parameters& params);
    void setDictParameters(const Parameters& params);

signals:
    void lookupReady(const QJsonDocument& result);
    void translateReady(const QJsonDocument& result);
    void getLangsReady(const QJsonDocument& result);
};


class MSWorker : public BaseWebWorker {};

#endif // NETWORK_H
