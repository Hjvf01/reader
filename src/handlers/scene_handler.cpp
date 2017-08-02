#include "handlers.h"


SelectMode SceneHandler::select_mode = SelectMode::translate;


SceneHandler::SceneHandler(const DocPtr& doc) : QObject(), document(doc) {
    scene = new DocScene;

    initConnectors();
}

SceneHandler::~SceneHandler() {
    /* Сцена удалится автоматически */
}


DocScene* SceneHandler::getScene() const { return scene; }

void SceneHandler::setLangFrom(const QString &lang) { lang_from = lang; }
void SceneHandler::setLangTo(const QString &lang) { lang_to = lang; }


void SceneHandler::initConnectors() {
    connector.set(scene, this);
    connector.connect<const QPointF&>(
        vector<void (DocScene::*)(const QPointF&)>{
            &DocScene::doubleClick
        },
        vector<void (SceneHandler::*)(const QPointF&)>{
            &SceneHandler::onDoubleClick
        }
    );

    scene2network.set(this, &translator);
    scene2network.connect<const Parameters&>(
        vector<void (SceneHandler::*)(const Parameters&)>{
            &SceneHandler::translate, &SceneHandler::lookup
        },
        vector<void (YandexWorker::*)(const Parameters&)>{
            &YandexWorker::onTranslate, &YandexWorker::onLookup
        }
    );

    network2handler.set(&translator, &network_handler);
    network2handler.connect<const QJsonDocument&>(
        vector<void (YandexWorker::*)(const QJsonDocument&)>{
            &YandexWorker::translateReady, &YandexWorker::lookupReady
        },
        vector<void (YandexHandler::*)(const QJsonDocument&)>{
            &YandexHandler::translateHandle, &YandexHandler::lookupHandle
        }
    );

    handler2this.set(&network_handler, this);
    handler2this.connect<const QString&>(
        vector<void (YandexHandler::*)(const QString&)>{
            &YandexHandler::translateRedy, &YandexHandler::lookupReady
        },
        vector<void (SceneHandler::*)(const QString&)>{
            &SceneHandler::onTranslateReady, &SceneHandler::onLookupReady
        }
    );

    connect(
        &dialog, &TrDialog::closeDialog, this, &SceneHandler::onDialogClose
    );
}


bool SceneHandler::pointBeyondScene(const QPointF &point) const {
    return (
        (point.x() > scene->sceneRect().width()) ||
        (point.y() > scene->sceneRect().height()) ||
        (point.x() < 0) ||
        (point.y() < 0)
    );
}


void SceneHandler::onDoubleClick(const QPointF& point) {
    if(pointBeyondScene(point))
        return;

    if(
        SceneHandler::select_mode == SelectMode::translate &&
        lang_from == QString() && lang_to == QString()
    ) return;

    QList<QGraphicsItem*> items = scene->items();
    for(QGraphicsItem* item: items) {
        if(item->contains(point)) {
            PageView* page = dynamic_cast<PageView*>(item);
            if(page == nullptr)
                continue;

            pair<QRectF, QString> word_box =
                document.get()->page(page->getIndex())->getTextBox(point);
            if(word_box.first.width() == 0 && word_box.first.height() == 0)
                return;
            scene->setHightLight(word_box.first);
            emitter(word_box.first, word_box.second);
            return;
        }
    }
}


void SceneHandler::onError(const QString& error_msg) { qDebug() << error_msg; }


void SceneHandler::onTranslateReady(const QString& result) {
    dialog.setTranslate(result);
    if(dialog_shown == false)
        dialog.show();
    dialog_shown = true;
}


void SceneHandler::onLookupReady(const QString& result) {
    dialog.setLookup(result);
    if(dialog_shown == false)
        dialog.show();
    dialog_shown = true;
}


void SceneHandler::onDialogClose(void) {
    scene->eraseHightlight();
    dialog_shown = false;
}


void SceneHandler::emitter(const QRectF &box, const QString& text) {
    switch(SceneHandler::select_mode) {
        case SelectMode::hightlight:
            emit drawFillRect(box);
            return;
        case SelectMode::underline:
            emit drawSolidLine(box);
            return;
        case SelectMode::dashed:
            emit drawDashedLine(box);
            return;
        case SelectMode::translate:
            dialog.setWindowTitle(text);
            emit translate(
                Parameters{
                    Parameter{"lang", lang_from + "-" + lang_to},
                    Parameter{"text", text}
                }
            );
            emit lookup(
                Parameters{
                    Parameter{"lang", lang_from + "-" + lang_to},
                    Parameter{"ui", lang_to},
                    Parameter{"text", text}
                }
            );
            return;
        case SelectMode::clipboard:
            emit copyToClipboard(text);
            return;
        default: return;
    }
}
