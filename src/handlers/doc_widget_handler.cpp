#include <string>
using std::to_string;

#include "handlers.h"


DocWidgetHandler::DocWidgetHandler(const QUrl& path) : QObject() {
    document = DocPtr(new PDFDocument(path.path(), path.fileName()));
    handler = new DocHandler(document);
    ui = new DocWidget;
    ui->setCentralWidget(handler->getView());

    doc_menu_h = new DocMenuHandler(path);
    ui->addDockWidget(Qt::LeftDockWidgetArea, doc_menu_h->getDocumentMenu());

    tool_bar = new DocToolBar(
        doc_menu_h->getDocumentMenu()->toggleViewAction()
    );
    tool_bar->setPageNums(
        QString("1"), QString::number(document.get()->amountPages())
    );
    ui->addToolBar(tool_bar);

    initConnectors();
}

DocWidgetHandler::~DocWidgetHandler() {
    delete tool_bar;
    delete handler;
    delete ui;
}


DocWidget* DocWidgetHandler::getWidget() const { return ui; }
DocHandler* DocWidgetHandler::getHandler() const { return handler; }
BaseDocument* DocWidgetHandler::getDocument() const { return document.get(); }


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

    combo_box_connector.set(tool_bar->getComboBoxes(), this);
    combo_box_connector.connect<const QString&>(
        vector<void (QComboBox::*)(const QString&)>{
            tool_bar->getComboBoxesAmount(),
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

    tool_bar_connector.set(tool_bar->getActions(), this);
    tool_bar_connector.connect<bool>(
        vector<void (QAction::*)(bool)>{
            tool_bar->getActionsAmount(),
            &QAction::triggered
        },
        vector<void (DocWidgetHandler::*)(bool)>{
            &DocWidgetHandler::onZoomIn, &DocWidgetHandler::onZoomOut,
            &DocWidgetHandler::onReload, &DocWidgetHandler::onFirstPage,
            &DocWidgetHandler::onPrevPage, &DocWidgetHandler::onNextPage,
            &DocWidgetHandler::onLastPage, &DocWidgetHandler::onFindDialogShow,
        }
    );

    connect(
        handler, &DocHandler::pageChange,
        this, static_cast<void (DocWidgetHandler::*)(unsigned int)>(
            &DocWidgetHandler::onPageChange
        )
    );
    connect(
        doc_menu_h, &DocMenuHandler::changePage,
        this, static_cast<void (DocWidgetHandler::*)(const QString&)>(
            &DocWidgetHandler::onPageChange
        )
    );
    connect(
        tool_bar->getPageLine(), &QLineEdit::textEdited,
        this, static_cast<void (DocWidgetHandler::*)(const QString&)>(
            &DocWidgetHandler::onPageChange
        )
    );

    connect(
        this, &DocWidgetHandler::getLangs, &langs, &YandexWorker::onGetLangs
    );
    connect(
        &langs, &YandexWorker::getLangsReady,
        &network_handler, &YandexHandler::langsHandle
    );
    connect(
        &network_handler, &YandexHandler::langsReady,
        this, &DocWidgetHandler::onDictLangsReady
    );

    connect(
        &find_dialog, &FindDialog::find, this, &DocWidgetHandler::onFind
    );

    connect(
        &find_dialog, &FindDialog::closeDialog,
        this, &DocWidgetHandler::onFindDialogClose
    );
}



            /* slots */
void DocWidgetHandler::onAbsoluteScaleChanged(const QString& value) {
    handler->resize(value.toInt());
}

void DocWidgetHandler::onZoomIn(bool) {
    #define INDEX        tool_bar->getScaleBox()->currentIndex()
    #define CURRENT_TEXT tool_bar->getScaleBox()->currentText()

    if(CURRENT_TEXT == QString("150"))
        return;
    tool_bar->getScaleBox()->setCurrentIndex(INDEX + 1);
}

void DocWidgetHandler::onZoomOut(bool) {
    if(CURRENT_TEXT == QString("50"))
        return;
    tool_bar->getScaleBox()->setCurrentIndex(INDEX - 1);

    #undef CURRENT_TEXT
    #undef INDEX
}

void DocWidgetHandler::onTrFromChanged(const QString& lang_name) {
    /* Устанавливает исходный язык и меняет список целевых */

    QComboBox* tr_to_box = tool_bar->getTrTo();
    int count = tr_to_box->count();
    for(int i = 0; i < count; i++)
        tr_to_box->removeItem(i);
    tr_to_box->addItems(dict_langs[lang_name]);

    handler->getSceneHandler()->setLangFrom(lang_name);
}

void DocWidgetHandler::onTrToChanged(const QString& lang_name) {
    /* устанавливает целевой язык */
    handler->getSceneHandler()->setLangTo(lang_name);
}


void DocWidgetHandler::onError(const QString& error_msg) {
    qDebug() << error_msg;
}

void DocWidgetHandler::onDictLangsReady(const Langs& result) {
    dict_langs = result;
    tool_bar->getTrFrom()->addItems(dict_langs.keys());
}

void DocWidgetHandler::onPageChange(unsigned int index) {
    tool_bar->setCurrentPage(QString::number(index + 1));
}

void DocWidgetHandler::onPageChange(const QString &page) {
    qDebug() << "Page: " << page;
    if(page == "") {
        tool_bar->setCurrentPage(
            QString::number(handler->getCurrentPage())
        );
        return;
    }
    int indx = page.toInt();
    if(indx == 0) return;
    handler->goTo((unsigned int)indx - 1);
}

void DocWidgetHandler::onNextPage(bool) { handler->goToNext(); }
void DocWidgetHandler::onPrevPage(bool) { handler->goToPrev(); }

void DocWidgetHandler::onFirstPage(bool) {
    if(document.get()->amountPages() == 1)
        return;
    if(handler->getCurrentPage() == 0)
        return;

    handler->goTo(0);
}

void DocWidgetHandler::onLastPage(bool) {
    unsigned int last = document.get()->amountPages() - 1;
    if(document.get()->amountPages() == 1)
        return;
    if(handler->getCurrentPage() == last)
        return;

    handler->goTo(last);
}

void DocWidgetHandler::onFullScreen(bool) {}


void DocWidgetHandler::onFind(const QString& text) {
    vector<pair<QRectF, QString>> results;
    vector<unsigned int> indexes = handler->getIndexes();

    for(unsigned int i: indexes) {
        auto _results = document.get()->page(i)->findAll(text);
        for(auto result: _results) {
            results.push_back(result);
        }
    }

    vector<QRectF> boxes;
    for(auto result: results)
        boxes.push_back(result.first);

    handler->getSceneHandler()->getScene()->setHightLights(boxes);
}

void DocWidgetHandler::onFindDialogClose() {
    handler->getSceneHandler()->getScene()->eraseHightlights();
}

void DocWidgetHandler::onFindDialogShow(bool) {
    find_dialog.show();
}


void DocWidgetHandler::onReload(bool) {
    if(tool_bar->getTrFrom()->count() > 0)
        return;

    emit getLangs();
}


void DocWidgetHandler::onDocementMenuShow(bool) {
    if(doc_menu_shown == false) {
        doc_menu_h->getDocumentMenu()->show();
        doc_menu_shown = true;
        return;
    }

    if(doc_menu_shown == true)
        return;
}

/* end slots */
