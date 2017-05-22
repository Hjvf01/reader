#include "handlers.h"


DocWidgetHandler::DocWidgetHandler(DocWidget* ui, DocHandler* h) : QObject() {
    this->ui = ui;
    this->handler = h;
    from_tr_lang = "";
    to_tr_lang = "";
    from_dict_lang = "";
    to_dict_lang = "";

    table_of_content = new QTreeView;
    toc_container = new QDockWidget;
    toc_container->setWidget(table_of_content);

    webConnector();
    uiConnector();
    connector();

    table_of_content->setModel(handler->getDoc()->getToc());
    ui->addDockWidget(Qt::LeftDockWidgetArea, toc_container);

    emit getDictLangs();
}

DocWidgetHandler::~DocWidgetHandler() {
    delete table_of_content;
    delete toc_container;
}


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
}

void DocWidgetHandler::connector() {
    /* Соединяет сигналы обработчика документа и его слоты */
    connect(
        handler, &DocHandler::pageChange, this, &DocWidgetHandler::onPageChange
    );
}



            /* slots */
void DocWidgetHandler::onAbsoluteScaleChanged(const QString& value) {
    qDebug() << value;
}

void DocWidgetHandler::onZoomIn() {
    #define INDEX ui->getToolBar()->getScaleBox()->currentIndex()
    ui->getToolBar()->getScaleBox()->setCurrentIndex(INDEX + 1);
}

void DocWidgetHandler::onZoomOut() {
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
    /* Меняет номер текущей страницы в представлении */
    qDebug() << "current page: " << index;
}
/* end slots */
