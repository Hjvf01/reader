#include "handlers.h"


DocWidgetHandler::DocWidgetHandler(const QUrl& path) : QObject() {
    handler = new DocHandler(DocPtr(
        new PDFDocument(path.path(), path.fileName())
    ));
    ui = new DocWidget;
    ui->setCentralWidget(handler->getView());
}

DocWidgetHandler::~DocWidgetHandler() {
    delete handler;
    delete ui;
}


DocWidget* DocWidgetHandler::getWidget() const { return ui; }
DocHandler* DocWidgetHandler::getHandler() const { return handler; }


void DocWidgetHandler::initConnectors() {
    context_menu_connector.set(ui->getContextMenu(), this);
    context_menu_connector.connect<bool>(
        vector<void (QAction::*)(bool)>{
            ui->getContextMenuSize(),
            &QAction::triggered
        },
        vector<void (DocWidgetHandler::*)(bool)>{
            &DocWidgetHandler::onFindDialogShow,
            &DocWidgetHandler::onFirstPage,
            &DocWidgetHandler::onPrevPage,
            &DocWidgetHandler::onNextPage,
            &DocWidgetHandler::onLastPage,
            &DocWidgetHandler::onZoomIn,
            &DocWidgetHandler::onZoomOut,
            &DocWidgetHandler::onFullScreen
        }
    );

    combo_box_connector.set(ui->getToolBar()->getComboBoxes(), this);
    combo_box_connector.connect<const QString&>(
        vector<void (QComboBox::*)(const QString&)>{
            ui->getToolBar()->getComboBoxesAmount(),
            static_cast<void (QComboBox::*)(const QString&)>(
                &QComboBox::currentIndexChanged
            )
        },
        vector<void (DocWidgetHandler::*)(const QString&)>{
            &DocWidgetHandler::onAbsoluteScaleChanged,
            &DocWidgetHandler::onTrFromChanged,
            &DocWidgetHandler::onTrToChanged,
        }
    );

    tool_bar_connector.set(ui->getToolBar()->getActions(), this);
    tool_bar_connector.connect<bool>(
        vector<void (QAction::*)(bool)>{
            ui->getToolBar()->getActionsAmount(),
            &QAction::triggered
        },
        vector<void (DocWidgetHandler::*)(bool)>{
            &DocWidgetHandler::onZoomIn,
            &DocWidgetHandler::onZoomOut,
            &DocWidgetHandler::onReload,
            &DocWidgetHandler::onFirstPage,
            &DocWidgetHandler::onPrevPage,
            &DocWidgetHandler::onNextPage,
            &DocWidgetHandler::onLastPage,
            &DocWidgetHandler::onFindDialogShow,
        }
    );
}



            /* slots */
void DocWidgetHandler::onAbsoluteScaleChanged(const QString& value) {
    handler->resize(value.toInt());
}

void DocWidgetHandler::onZoomIn(bool) {
    #define INDEX        ui->getToolBar()->getScaleBox()->currentIndex()
    #define CURRENT_TEXT ui->getToolBar()->getScaleBox()->currentText()

    if(CURRENT_TEXT == QString("150"))
        return;
    ui->getToolBar()->getScaleBox()->setCurrentIndex(INDEX + 1);
}

void DocWidgetHandler::onZoomOut(bool) {
    if(CURRENT_TEXT == QString("50"))
        return;
    ui->getToolBar()->getScaleBox()->setCurrentIndex(INDEX - 1);

    #undef CURRENT_TEXT
    #undef INDEX
}

void DocWidgetHandler::onTrFromChanged(const QString& lang_name) {
    /* Устанавливает исходный язык и меняет список целевых */

    QComboBox* tr_to_box = ui->getToolBar()->getTrTo();
    int count = tr_to_box->count();
    for(int i = 0; i < count; i++)
        tr_to_box->removeItem(i);
    tr_to_box->addItems(dict_langs[lang_name]);
}

void DocWidgetHandler::onTrToChanged(const QString& lang_name) {
    /* устанавливает целевой язык */
}

void DocWidgetHandler::onTranslate(const QString& text) {
    /* формирует запрос перевода слова
    emit translate(text, {
        Parametr("lang", from_tr_lang + "-" + to_tr_lang)
    });
    */
}

void DocWidgetHandler::onLookup(const QString& text) {
    /* формирует запрос поиска в словаре
    emit lookup(text, {
        Parametr("lang", from_dict_lang + "-" + to_dict_lang),
        Parametr("ui", to_dict_lang)
    });
    */
}

void DocWidgetHandler::onError(const QString& error_msg) {
    qDebug() << error_msg;
}

void DocWidgetHandler::onDictLangsReady(const QJsonArray& result) {
    /* Формирует список пар (исходный язык - целевой) для возможности
     * выбора их пользователем */
    #define SOURCE_LIST ui->getToolBar()->getTrFrom()

    QMultiMap<QString, QString> d_langs;
    QList<QVariant> langs = result.toVariantList();
    for(QVariant lang: langs) {
        QStringList pair = lang.toString().split("-");
        QString key = pair[0];
        QString value = pair[1];
        d_langs.insert(key, value);
    }
    for(QString key: d_langs.keys()) dict_langs.insert(key, {});

    for(QString key: dict_langs.keys()) {
        auto entry = d_langs.find(key);
        while(entry != d_langs.end() && entry.key() == key) {
            dict_langs[key].append(entry.value());
            entry++;
        }
    }
    SOURCE_LIST->addItems(dict_langs.keys());

    #undef SOURCE_LIST
}

void DocWidgetHandler::onPageChange(unsigned int index) {
    ui->getToolBar()->setCurrentPage(QString::number(index + 1));
}

void DocWidgetHandler::onChangePage(const QString &page) {
    if(page == "") {
        ui->getToolBar()->setCurrentPage(
            QString::number(handler->getCurrentPage())
        );
        return;
    }
    int indx = page.toInt();
    if(indx == 0) return;
    handler->goTo((unsigned int)indx - 1);
}

void DocWidgetHandler::onNextPage(bool) {
    unsigned int i = handler->getCurrentPage();
    //if(i == handler->getDoc()->amountPages() - 1) return;
    handler->goTo(i + 1);
}

void DocWidgetHandler::onPrevPage(bool) {
    unsigned int i = handler->getCurrentPage();
    if(i == 0) return;
    onChangePage(QString::number(i));
}

void DocWidgetHandler::onFirstPage(bool) {
    //if(handler->getDoc()->amountPages() == 1)
        return;
    handler->goTo(0);
}

void DocWidgetHandler::onLastPage(bool) {
    //if(handler->getDoc()->amountPages() == 1)
        return;
    //handler->goTo(handler->getDoc()->amountPages() - 1);
}

void DocWidgetHandler::onFullScreen(bool) {}


void DocWidgetHandler::onFindDialogShow(bool) { dialog.show(); }

void DocWidgetHandler::onFind(const QString& text) {
    vector<pair<QRectF, QString>> results;
    vector<unsigned int> indexes = handler->getIndexes();
/*
    for(unsigned int i: indexes)
        for(auto result: handler->getDoc()->page(i)->findAll(text)) {
            results.push_back(result);
        }

    vector<QRectF> boxes;
    for(auto result: results)
        boxes.push_back(result.first);

    ui->getView()->getScene()->setHightLights(boxes);*/
}

void DocWidgetHandler::onFindDialogClose() {
    //ui->getView()->getScene()->eraseHightlights();
}


void DocWidgetHandler::onReload(bool) {
    if(ui->getToolBar()->getTrFrom()->count() > 0)
        return;
    emit getDictLangs();
}
/* end slots */
