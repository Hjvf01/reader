#include "handlers.h"


DocWidgetHandler::DocWidgetHandler(DocWidget* ui, DocHandler* h) : QObject() {
    this->ui = ui;
    this->handler = h;
    from_tr_lang = "";
    to_tr_lang = "";
    from_dict_lang = "";
    to_dict_lang = "";

    table_of_content = new QTreeView;

    webConnector();
    uiConnector();
    connector();

    table_of_content->setModel(handler->getDoc()->getToc());
    ui->setLeftDock(table_of_content);

    emit getDictLangs();

    ui->getToolBar()->setPageNums(
        QString::number(handler->getCurrentPage() + 1),
        QString::number(handler->getDoc()->amountPages())
    );
}

DocWidgetHandler::~DocWidgetHandler() {}


DocWidget* DocWidgetHandler::getWidget() const { return ui; }


void DocWidgetHandler::webConnector() {
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

void DocWidgetHandler::uiConnector() {
    /* Соединяет сигналы представления этого обработчика и его слоты */
   QComboBox* s = ui->getToolBar()->getScaleBox();

    connect(
        s, static_cast<void(QComboBox::*)(const QString &)>
            (&QComboBox::currentIndexChanged),
        this, &DocWidgetHandler::onAbsoluteScaleChanged
    );

    s = ui->getToolBar()->getTrFrom();
    connect(
        s, static_cast<void(QComboBox::*)(const QString &)>
            (&QComboBox::currentIndexChanged),
        this, &DocWidgetHandler::onTrFromChanged
    );
    s = ui->getToolBar()->getTrTo();
    connect(
        s, static_cast<void(QComboBox::*)(const QString &)>
            (&QComboBox::currentIndexChanged),
        this, &DocWidgetHandler::onTrToChanged
    );

    connect(
        ui->getToolBar()->getZoomIn(), &QAction::triggered,
        this, &DocWidgetHandler::onZoomIn
    );
    connect(
        ui->getToolBar()->getZoomOut(), &QAction::triggered,
        this, &DocWidgetHandler::onZoomOut
    );
    connect(
        ui->getToolBar()->getCurrentPage(), &QLineEdit::textEdited,
        this, &DocWidgetHandler::onChangePage
    );

    connect(
        ui, &DocWidget::menuConnectSignal,
        this, &DocWidgetHandler::onContextMenuConnector
    );
}

void DocWidgetHandler::connector() {
    /* Соединяет сигналы обработчика документа и его слоты */
    connect(
        handler, &DocHandler::pageChange, this, &DocWidgetHandler::onPageChange
    );
    connect(
        table_of_content, &QTreeView::activated,
        this, &DocWidgetHandler::onTOCActivated
    );
}


            /* slots */
void DocWidgetHandler::onContextMenuConnector(DocContextMenu *menu) {
    connect(
        menu->getFirstPage(), &QAction::triggered,
        this, &DocWidgetHandler::onFirstPage
    );
    connect(
        menu->getLastPage(), &QAction::triggered,
        this, &DocWidgetHandler::onLastPage
    );
    connect(
        menu->getNextPage(), &QAction::triggered,
        this, &DocWidgetHandler::onNextPage
    );
    connect(
        menu->getPrevPage(), &QAction::triggered,
        this, &DocWidgetHandler::onPrevPage
    );
    connect(
        menu->getZoomIn(), &QAction::triggered,
        this, &DocWidgetHandler::onZoomIn
    );
    connect(
        menu->getZoomOut(), &QAction::triggered,
        this, &DocWidgetHandler::onZoomOut
    );
    connect(
        menu->getFullScreen(), &QAction::triggered,
        this, &DocWidgetHandler::onFullScreen
    );
}

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
    #define INDEX ui->getToolBar()->getScaleBox()->currentIndex()
    if(ui->getToolBar()->getScaleBox()->currentText() == QString("150"))
        return;
    ui->getToolBar()->getScaleBox()->setCurrentIndex(INDEX + 1);
}

void DocWidgetHandler::onZoomOut() {
    if(ui->getToolBar()->getScaleBox()->currentText() == QString("50"))
        return;
    ui->getToolBar()->getScaleBox()->setCurrentIndex(INDEX - 1);
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
    ui->getToolBar()->getTrFrom()->addItems(dict_langs.keys());

    #undef AND
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
    onChangePage(QString::number(i + 1));
}

void DocWidgetHandler::onPrevPage() {
    unsigned int i = handler->getCurrentPage();
    if(i == 0) return;
    onChangePage(QString::number(i - 1));
}

void DocWidgetHandler::onFirstPage() {
    onChangePage(QString::number(0));
}

void DocWidgetHandler::onLastPage() {
    qDebug() << "lkj";
    onChangePage(QString::number(handler->getDoc()->amountPages() - 1));
}

void DocWidgetHandler::onFullScreen() {}
/* end slots */
