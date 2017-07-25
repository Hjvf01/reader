#ifndef PARSERS_H
#define PARSERS_H


#include <QtCore/QObject>
#include <QtCore/QMap>
#include <QtCore/QPair>
#include <QtCore/QMultiMap>
#include <QtCore/QString>
#include <QtCore/QJsonDocument>


class YandexParser : public QObject {
    Q_OBJECT

    QMap<QString, QString> langs;
    QString lookup_result;
    QString translate_result;

public:
    YandexParser();
    ~YandexParser() override;

public slots:
    void langsParse(const QJsonDocument& result);
    void lookupParse(const QJsonDocument& result);
    void translateParse(const QJsonDocument& result);

signals:
    void langsReady(const QMap<QString, QString> _langs);
    void lookupReady(const QString& _result);
    void translateRedy(const QString& _result);
};

#endif // PARSERS_H
