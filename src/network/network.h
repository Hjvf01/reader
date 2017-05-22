#ifndef NETWORK_H
#define NETWORK_H

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


class BaseWebWorker : public QObject {
    Q_OBJECT

protected:
    using Prm = QPair<QString, QString>;
    using Prms = QList<Prm>;

    QNetworkAccessManager manager;
    QNetworkRequest request;
    QUrlQuery params;
    QUrl url;
    QSslConfiguration config;

public:
    BaseWebWorker() {
        config = QSslConfiguration::defaultConfiguration();
        config.setProtocol(QSsl::TlsV1_2);
        request.setSslConfiguration(config);
    }

    ~BaseWebWorker() {}

    void setParams(Prms ps) {
        for(Prm param: params.queryItems()) {
            if(param.first != QString("key"))
                params.removeQueryItem(param.first);
        }
        for(Prm param: ps)
            params.addQueryItem(param.first, param.second);
    }

signals:
    /* если что-то пойдет не так*/
    void errorSignal(QString msg);
};


class TrWorker : public BaseWebWorker {
    Q_OBJECT

    const QString TR_KEY = "trnsl.1.1.20170510T212921Z."
                           "548f553bcaead33f"
                           ".e006a894fb485b094ae287188089d6a460d61ae5";

    const QString base_tr_url =
        "https://translate.yandex.net/api/v1.5/tr.json";

public:
    TrWorker() : BaseWebWorker() {
        params.addQueryItem("key", TR_KEY);

        QObject::connect(
            &manager, &QNetworkAccessManager::finished,
            this, &TrWorker::onFinished
        );
    }
    ~TrWorker() {}

public slots:
    void onGetLangs(void) {
        /*
         *      Метод вернет список языков поддерживаемых переводчиком
            1. key - API-key
            2. ui* - user language
        */
        QUrl url(base_tr_url + "/getLangs");
        url.setQuery(params.query());
        request.setUrl(url);
        manager.get(request);
    }

    void onTranslate(const QString txt, Prms prms) {
        /*
         *      Метод вернет перевод слова/выражения с исходного
         *      языка на целевой
            key - API-key(from db)
            text - text to translate(from arg)
            lang - from-to
            format* - plain | html
        */
        setParams(prms);
        params.addQueryItem("text", txt);
        QUrl url(base_tr_url + "/translate");
        url.setQuery(params.query());
        request.setUrl(url);
        manager.get(request);
    }

private slots:
    void onFinished(QNetworkReply* reply) {
        #define NOT !

        QJsonParseError error;
        QJsonDocument resp = QJsonDocument::fromJson(
            reply->readAll(), &error
        );

        if(error.error == 0) {
            if(resp.isObject()) {
                QJsonObject result = resp.object();
                if(NOT result["dirs"].isNull())
                    emit getLangsReady(result);
                else
                    emit translateReady(result);
            } else if(resp.isNull())
                emit errorSignal("Internet disconnected");
            else
                qDebug() << "Undefined";
        } else qDebug() << error.errorString();

        reply->close();
        delete reply;
        #undef NOT
    }

signals:
    void getLangsReady(const QJsonObject result);
    void translateReady(const QJsonObject result);
};


class DictWorker : public BaseWebWorker {
    Q_OBJECT

    const QString DICT_KEY = "dict.1.1.20170512T161717Z"
                             ".ad55f59bb8ee8774"
                             ".1c3bd8fb768f39144b375917853fddb2c4203382";
    const QString base_dict_url =
        "https://dictionary.yandex.net/api/v1/dicservice.json";

public:
    DictWorker() : BaseWebWorker() {
        params.addQueryItem("key", DICT_KEY);

        QObject::connect(
            &manager, &QNetworkAccessManager::finished,
            this, &DictWorker::onFinished
        );
    }
    ~DictWorker() {}

public slots:
    void onGetLangs(void) {
        /*
            Метод вернет список языков поддерживаемых Яндекс словарем
            key - API-key
        */
        QUrl url(base_dict_url + "/getLangs");
        url.setQuery(params.query());
        request.setUrl(url);
        manager.get(request);
    }

    void onLookup(const QString txt, Prms prms) {
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
        setParams(prms);
        params.addQueryItem("text", txt);
        QUrl url(base_dict_url + "/lookup");
        url.setQuery(params.query());
        request.setUrl(url);
        manager.get(request);
    }

private slots:
    void onFinished(QNetworkReply* reply) {
        QJsonParseError error;
        QJsonDocument resp = QJsonDocument::fromJson(
            reply->readAll(), &error
        );

        if(error.error == 0) {
            if(resp.isObject()) {
                emit lookupReady(resp.object());
            } else if(resp.isArray()) {
                emit getLangsReady(resp.array());
            } else if(resp.isNull())
                errorSignal("Intrnet disconnected");
            else
                qDebug() << "Undefined";
        } else
            emit errorSignal(error.errorString());

        reply->close();
        delete reply;
    }

signals:
    void getLangsReady(const QJsonArray langs);
    void lookupReady(const QJsonObject result);
};

#endif // NETWORK_H
