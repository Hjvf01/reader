#include "handlers.h"


DoubleClickMode SceneHandler::dbclick_mode = DoubleClickMode::translate;


SceneHandler::SceneHandler(DocPtr doc) : QObject() {
    scene = new DocScene;
    document = doc;

    initConnectors();
}

SceneHandler::~SceneHandler() {
    /* Сцена удалится автоматически */
}


DocScene* SceneHandler::getScene() const { return scene; }


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
            dialog.setWindowTitle(word_box.second);
            return;
        }
    }
}


void SceneHandler::onError(const QString& error_msg) { qDebug() << error_msg; }


void SceneHandler::onTranslateReady(const QJsonObject& result) {
    dialog.setTranslate(result);
    if(dialog_shown == false)
        dialog.show();
    dialog_shown = true;
}


void SceneHandler::onLookupReady(const QJsonObject& result) {
    dialog.setLookup(result);
    if(dialog_shown == false)
        dialog.show();
    dialog_shown = true;
}


void SceneHandler::emitter(const QRectF &box, const QString& text) {
    switch(SceneHandler::dbclick_mode) {
        case DoubleClickMode::hightlight:
            break;
        case DoubleClickMode::underline:
            break;
        case DoubleClickMode::dashed:
            break;
        case DoubleClickMode::translate:
            emit translate(text);
            emit lookup(text);
            return;
        case DoubleClickMode::clipboard:
            break;
        default: return;
    }
}
