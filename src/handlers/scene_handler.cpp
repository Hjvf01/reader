#include "handlers.h"


SceneHandler::SceneHandler(DocPtr doc) : QObject() {
    scene = new DocScene;
    document = doc;

    initConnectors();
}

SceneHandler::~SceneHandler() {
    //delete scene;
}


DocScene* SceneHandler::getScene() const { return scene; }


void SceneHandler::initConnectors() {
    connector.set(scene, this);
    connector.connect<const QPointF&>(scene->getSceneSignals(), handlers());
}


bool SceneHandler::pointBeyondScene(const QPointF &point) {
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

    QList<QGraphicsItem*> pages = scene->items();
    for(QGraphicsItem* page: pages) {
        if(page->contains(point)) {
            pair<QRectF, QString> word_box =
                document.get()->page(
                    static_cast<PageView*>(page)->getIndex())->getTextBox(point);

            if(word_box.first.width() == 0 && word_box.first.height() == 0)
                return;
            scene->setHightLight(word_box.first);

            emit translate(word_box.second);
            emit lookup(word_box.second);

            dialog.setWindowTitle(word_box.second);

            return;
        }
    }
}


void SceneHandler::onError(const QString& error_msg) {
    qDebug() << error_msg;
}


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


const vector<void (SceneHandler::*)(const QPointF&)>
        SceneHandler::handlers() const {
    return {
        &SceneHandler::onDoubleClick
    };
}
