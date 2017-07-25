#ifndef NETWORK_HANDLERS_H
#define NETWORK_HANDLERS_H


#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QString>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonObject>


class YandexHandler : public QObject {
    Q_OBJECT

public:
    YandexHandler();
    ~YandexHandler() override;

private:
    QString getPOS(const QJsonObject& pos);
    QString getTr(const QJsonArray& trs);
    QString getMeans(const QJsonArray& means);

public slots:
    void langsHandle(const QJsonDocument& result);
    void lookupHandle(const QJsonDocument& result);
    void translateHandle(const QJsonDocument& result);

signals:
    void langsReady(const QMap<QString, QList<QString>>& langs);
    void lookupReady(const QString& result);
    void translateRedy(const QString& result);
};

#endif // NETWORK_HANDLERS_H
