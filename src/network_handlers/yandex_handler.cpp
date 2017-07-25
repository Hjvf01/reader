#include <cassert>
#include <algorithm>
using std::move;

#include <QtCore/QJsonArray>
#include <QtCore/QMultiMap>
#include <QtCore/QMap>
#include <QtCore/QList>
#include <QtCore/QDebug>

#include "network_handlers.h"


YandexHandler::YandexHandler() : QObject() {}
YandexHandler::~YandexHandler() {}


void YandexHandler::langsHandle(const QJsonDocument &result) {
    /*
                    INPUT
        ["be-be","be-ru", ... "tt-ru","uk-en","uk-ru","uk-uk"]

                    OUTPUT
        QMap<QString, QList<QString>> out = {
            QPair<QString, QList<QString>(
                "be", QList<QString>("be", "ru", ...)
            ),
            ...
            QPair<QString, QList<QString>>(
                "uk", QList<QString>(..., "en", "ru", "uk")
            )
        }
    */

    QMap<QString, QList<QString>> langs;

    QMultiMap<QString, QString> _langs;
    QList<QVariant> langs_list = result.array().toVariantList();
    for(QVariant lang: langs_list) {
        QStringList pair = lang.toString().split("-");
        assert(pair.size() == 2);
        _langs.insert(pair[0], pair[1]);
    }

    for(QString key: _langs.keys())
        langs.insert(key, {});

    for(QString key: langs.keys()) {
        auto entry = _langs.find(key);
        while(entry != _langs.end() && entry.key() == key) {
            langs[key].append(entry.value());
            ++entry;
        }
    }

    emit langsReady(langs);
}


void YandexHandler::lookupHandle(const QJsonDocument &result) {
    /*
                Scheme of input
        resp = {'def': [list with part of speech], 'head': {empty}}
        def = [1. POS#1, 2. POS#2, 3. POS#3, ..., N. POS#N]
        POS = {
            'pos': <part of speech in target language>
            'text': <word>
            'ts': <transcription>
            'tr': <examples of translation>
        }
        'tr' = [
            {
                'mean': [{'text': <word in source language>}, ...],
                'pos': <part of speech>,
                'syn': IGNORE,
                'text': <translation>
            }, ...
        ]
    */

    QString out = "<html><ol>";

    QJsonArray pos_array = result.object()["def"].toArray();
    for(QJsonValue elem: pos_array) {
        out += getPOS(elem.toObject());
        qDebug() << out;
    }
    out += "</ol></html>";

    emit lookupReady(out);
}


QString YandexHandler::getMeans(const QJsonArray &means) {
    /*
            INPUT
        [{'text': <word in source lang>}, ...]
            OUTPUT
        (<word>, <word>, ...)
    */
    if(means.size() == 0)
        return "";

    if(means.size() == 1)
        return "<br><i>(" + means[0].toObject()["text"].toString() + ")</i>";

    return "<br><i>(" + means[0].toObject()["text"].toString() + ", "
        + means[1].toObject()["text"].toString() + ")</i>";
}


QString YandexHandler::getTr(const QJsonArray &trs) {
    /*
        'tr' = [
            {
                'mean': [{'text': <word in source language>}, ...],
                'pos': <part of speech>,
                'syn': IGNORE,
                'text': <translation>
            }, ...
        ]
    */

    QString out = "<ul>";
    for(QJsonValue elem: trs) {
        if(elem.isObject()) {
            QJsonObject object = elem.toObject();
            out += "<li>" + object["text"].toString()
                + getMeans(object["mean"].toArray()) + "</li>";
        }
    }
    return out + "</ul>";
}


QString YandexHandler::getPOS(const QJsonObject& pos) {
    /*
                INPUT
        POS = {
            'pos': <part of speech in target language>
            'text': <word>
            'ts': <transcription>
            'tr': <examples of translation>
        }
                OUTPUT
        <ol>
            <li>pos[text] - [pos[ts]] <small>(pos[pos])</small>
                pos[tr]
            </li>
            ...
        </ol>
    */
    return "<li><strong>" + pos["text"].toString() + " - "
        + "[" +pos["ts"].toString() + "] <br>"
        + "<small>(" + pos["pos"].toString()
        + ")</small></strong></li>"
        + getTr(pos["tr"].toArray());
}


void YandexHandler::translateHandle(const QJsonDocument &result) {
    emit translateRedy(QString("<html><h3>") +
        result.object()["text"]
        .toArray()
        .toVariantList()[0]
        .toString()
        + QString("</h3></html>")
    );
}
