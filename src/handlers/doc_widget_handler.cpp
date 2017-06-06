#include "handlers.h"


DocWidgetHandler::DocWidgetHandler(DocWidget* ui, DocHandler* h) : QObject() {
    this->ui = ui;
    this->handler = h;
    from_tr_lang = "";
    to_tr_lang = "";
    from_dict_lang = "";
    to_dict_lang = "";

    table_of_content = new QTreeView;

    widgetConnectors();
    toolBarConnectors();

    table_of_content->setModel(handler->getDoc()->getToc());
    ui->setLeftDock(table_of_content);

    ui->getToolBar()->setPageNums(
        QString::number(handler->getCurrentPage() + 1),
        QString::number(handler->getDoc()->amountPages())
    );
}

DocWidgetHandler::~DocWidgetHandler() {
    //table_of_content удаляется автоматически
}


DocWidget* DocWidgetHandler::getWidget() const { return ui; }


void DocWidgetHandler::translatorConnector() {
    /* Соединяет сигналы обработчика представления документа и переводчика */
    connect(
        handler, &DocHandler::translate,
        this, &DocWidgetHandler::onTranslate
    );
    connect(
        handler, &DocHandler::lookup,
        this, &DocWidgetHandler::onLookup
    );
    connect(
        this, &DocWidgetHandler::translate, &trnsl, &TrWorker::onTranslate
    );
    connect(
        this, &DocWidgetHandler::lookup, &dict, &DictWorker::onLookup
    );

    connect(
        &trnsl, &TrWorker::errorSignal, this, &DocWidgetHandler::onError
    );
    connect(
        &trnsl, &TrWorker::translateReady,
        handler, &DocHandler::onTranslateReady
    );

    connect(
        &dict, &DictWorker::errorSignal, this, &DocWidgetHandler::onError
    );
    connect(
        &dict, &DictWorker::lookupReady,
        handler, &DocHandler::onLookupReady
    );

    connect(
        this, &DocWidgetHandler::getDictLangs,
        &dict, &DictWorker::onGetLangs
    );

    connect(
        &dict, &DictWorker::getLangsReady,
        this, &DocWidgetHandler::onDictLangsReady
    );
}


void DocWidgetHandler::scaleMenuConnector() {
    #define TOOLBAR   ui->getToolBar()
    #define SCALE_BOX TOOLBAR->getScaleBox()
    #define ZOOM_IN   TOOLBAR->getZoomIn()
    #define ZOOM_OUT  TOOLBAR->getZoomOut()

    connect(
        SCALE_BOX, static_cast<void(QComboBox::*)(const QString &)>
        (&QComboBox::currentIndexChanged),
        this, &DocWidgetHandler::onAbsoluteScaleChanged
    );
    connect(
        ZOOM_IN, &QAction::triggered, this, &DocWidgetHandler::onZoomIn
    );
    connect(
        ZOOM_OUT, &QAction::triggered, this, &DocWidgetHandler::onZoomOut
    );

    #undef ZOOM_OUT
    #undef ZOOM_IN
    #undef SCALE_BOX
}

void DocWidgetHandler::translatorMenuConnector() {
    #define SOURCE_LIST TOOLBAR->getTrFrom()
    #define TARGET_LIST TOOLBAR->getTrTo()
    #define RELOAD      TOOLBAR->getReload()

    connect(
        RELOAD, &QAction::triggered, this, &DocWidgetHandler::onReload
    );
    connect(
        SOURCE_LIST, static_cast<void(QComboBox::*)(const QString &)>
            (&QComboBox::currentIndexChanged),
        this, &DocWidgetHandler::onTrFromChanged
    );
    connect(
        TARGET_LIST, static_cast<void(QComboBox::*)(const QString &)>
            (&QComboBox::currentIndexChanged),
        this, &DocWidgetHandler::onTrToChanged
    );

    #undef SOURCE_LIST
    #undef TARGET_LIST
    #undef RELOAD
}

void DocWidgetHandler::pageNavConnector() {
    #define FST_PAGE     TOOLBAR->getFstPage()
    #define PREV_PAGE    TOOLBAR->getPrevPage()
    #define NEXT_PAGE    TOOLBAR->getNextPage()
    #define LST_PAGE     TOOLBAR->getLastPage()
    #define CURRENT_PAGE TOOLBAR->getCurrentPage()
    #define FIND         TOOLBAR->getFind()

    connect(
        FST_PAGE, &QAction::triggered, this, &DocWidgetHandler::onFirstPage
    );
    connect(
        PREV_PAGE, &QAction::triggered, this, &DocWidgetHandler::onPrevPage
    );
    connect(
        NEXT_PAGE, &QAction::triggered, this, &DocWidgetHandler::onNextPage
    );
    connect(
        LST_PAGE, &QAction::triggered, this, &DocWidgetHandler::onLastPage
    );
    connect(
        FIND, &QAction::triggered, this, &DocWidgetHandler::onFindDialogShow
    );
    connect(
        CURRENT_PAGE, &QLineEdit::textEdited,
        this, &DocWidgetHandler::onChangePage
    );

    #undef FIND
    #undef CURRENT_PAGE
    #undef LST_PAGE
    #undef NEXT_PAGE
    #undef PREV_PAGE
    #undef FST_PAGE
    #undef TOOLBAR
}

void DocWidgetHandler::docHandlerConnector() {
    /* Соединяет сигналы обработчика документа и его слоты */
    connect(
        handler, &DocHandler::pageChange, this, &DocWidgetHandler::onPageChange
    );
    connect(
        table_of_content, &QTreeView::activated,
        this, &DocWidgetHandler::onTOCActivated
    );
    connect(&dialog, &FindText::findText, this, &DocWidgetHandler::onFind);
    connect(
        &dialog, &FindText::closeDialog,
        this, &DocWidgetHandler::onFindDialogClose
    );
}

void DocWidgetHandler::contextMenuConnector() {
    connect(
        ui->getFindAction(), &QAction::triggered,
        this, &DocWidgetHandler::onFindDialogShow
    );
    connect(
        ui->getFirstPage(), &QAction::triggered,
        this, &DocWidgetHandler::onFirstPage
    );
    connect(
        ui->getPrevPage(), &QAction::triggered,
        this, &DocWidgetHandler::onPrevPage
    );
    connect(
        ui->getNextPage(), &QAction::triggered,
        this, &DocWidgetHandler::onNextPage
    );
    connect(
        ui->getLastPage(), &QAction::triggered,
        this, &DocWidgetHandler::onLastPage
    );
    connect(
        ui->getZoomIn(), &QAction::triggered,
        this, &DocWidgetHandler::onZoomIn
    );
    connect(
        ui->getZoomOut(), &QAction::triggered,
        this, &DocWidgetHandler::onZoomOut
    );
    connect(
        ui->getFullScreen(), &QAction::triggered,
        this, &DocWidgetHandler::onFullScreen
    );
}

void DocWidgetHandler::toolBarConnectors() {
    scaleMenuConnector();
    translatorMenuConnector();
    pageNavConnector();
}

void DocWidgetHandler::widgetConnectors() {
    docHandlerConnector();
    contextMenuConnector();
    translatorConnector();
}


            /* slots */
void DocWidgetHandler::onTOCActivated(const QModelIndex &index) {
    try {
        int indx = handler->getDoc()->getPage(index);
        handler->goTo(indx);
    } catch(char const* err) {
        qDebug() << err;
    }
}

void DocWidgetHandler::onAbsoluteScaleChanged(const QString& value) {
    handler->resize(value.toInt());
}

void DocWidgetHandler::onZoomIn() {
    #define INDEX        ui->getToolBar()->getScaleBox()->currentIndex()
    #define CURRENT_TEXT ui->getToolBar()->getScaleBox()->currentText()

    if(CURRENT_TEXT == QString("150"))
        return;
    ui->getToolBar()->getScaleBox()->setCurrentIndex(INDEX + 1);
}

void DocWidgetHandler::onZoomOut() {
    if(CURRENT_TEXT == QString("50"))
        return;
    ui->getToolBar()->getScaleBox()->setCurrentIndex(INDEX - 1);

    #undef CURRENT_TEXT
    #undef INDEX
}

void DocWidgetHandler::onTrFromChanged(const QString& lang_name) {
    /* Устанавливает исходный язык и меняет список целевых */
    from_tr_lang = lang_name;
    from_dict_lang = lang_name;

    QComboBox* tr_to_box = ui->getToolBar()->getTrTo();
    int count = tr_to_box->count();
    for(int i = 0; i < count; i++)
        tr_to_box->removeItem(i);
    tr_to_box->addItems(dict_langs[lang_name]);
}

void DocWidgetHandler::onTrToChanged(const QString& lang_name) {
    /* устанавливает целевой язык */
    to_tr_lang = lang_name;
    to_dict_lang = lang_name;
}

void DocWidgetHandler::onTranslate(const QString text) {
    /* формирует запрос перевода слова */
    emit translate(text, {
        Parametr("lang", from_tr_lang + "-" + to_tr_lang)
    });
}

void DocWidgetHandler::onLookup(const QString text) {
    /* формирует запрос поиска в словаре */
    emit lookup(text, {
        Parametr("lang", from_dict_lang + "-" + to_dict_lang),
        Parametr("ui", to_dict_lang)
    });
}

void DocWidgetHandler::onError(QString error_msg) {
    qDebug() << error_msg;
}

void DocWidgetHandler::onDictLangsReady(const QJsonArray result) {
    /* Формирует список пар (исходный язык - целевой) для возможности
     * выбора их пользователем */
    #define AND &&
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
        while(entry != d_langs.end() AND entry.key() == key) {
            dict_langs[key].append(entry.value());
            entry++;
        }
    }
    SOURCE_LIST->addItems(dict_langs.keys());

    #undef AND
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

void DocWidgetHandler::onNextPage() {
    unsigned int i = handler->getCurrentPage();
    if(i == handler->getDoc()->amountPages() - 1) return;
    handler->goTo(i + 1);
}

void DocWidgetHandler::onPrevPage() {
    unsigned int i = handler->getCurrentPage();
    if(i == 0) return;
    onChangePage(QString::number(i));
}

void DocWidgetHandler::onFirstPage() {
    if(handler->getDoc()->amountPages() == 1) return;
    handler->goTo(0);
}

void DocWidgetHandler::onLastPage() {
    if(handler->getDoc()->amountPages() == 1) return;
    handler->goTo(handler->getDoc()->amountPages() - 1);
}

void DocWidgetHandler::onFullScreen() {}


void DocWidgetHandler::onFindDialogShow() { dialog.show(); }

void DocWidgetHandler::onFind(const QString text) {
    vector<pair<QRectF, QString>> results;
    vector<unsigned int> indexes = handler->getIndexes();

    for(unsigned int i: indexes)
        for(auto result: handler->getDoc()->page(i)->findAll(text)) {
            results.push_back(result);
        }

    vector<QRectF> boxes;
    for(auto result: results)
        boxes.push_back(result.first);

    ui->getView()->getScene()->setHightLights(boxes);
}

void DocWidgetHandler::onFindDialogClose() {
    ui->getView()->getScene()->eraseHightlights();
}


void DocWidgetHandler::onReload() {
    if(ui->getToolBar()->getTrFrom()->count() > 0)
        return;
    emit getDictLangs();
}
/* end slots */
