#ifndef HANDLERS_H
#define HANDLERS_H

#include <functional>
using std::function;
#include <deque>
using std::deque;

#include <QtWidgets/QGraphicsPixmapItem>

#include "../ui/ui.h"
#include "../model/models.h"
#include "../network/network.h"
#include "../ui/dialogs.h"
#include "../conectors/connectors.h"


class DocHandler : public QObject {
    Q_OBJECT

    using Index = unsigned int;
    using PagePixPtr = QGraphicsPixmapItem*;

    struct Page {
        Index index;
        PagePixPtr page;

        Page(Index i, PagePixPtr p) : index(i), page(p) {}
        ~Page() { delete page; }
    };

    using PagePtr = Page*;

    Index buf_size = 3;
    DocView* ui;
    Index current;
    deque<PagePtr> pages;
    BaseDocument* document;
    int location;
    double scale_factor;
    TrDialog tr_dialog;
    bool dialog_shown = false;

    One2One<DocView, DocHandler>* scrolling_connector;
    One2One<DocScene, DocHandler>* scene_connector;

public:
    DocHandler(DocView* widget, BaseDocument* doc);
    ~DocHandler();

    unsigned int getCurrentPage(void) const;
    int getLocation(void) const;

    DocView* getView(void) const;
    BaseDocument* getDoc(void) const;
    deque<Page*> getPages() const { return pages; }

    void resize(int new_value);

    vector<Index> getIndexes(void);

    void goTo(unsigned int index);

public slots:
    void onDoubleClick(QPointF point);

    void onScrollUp(int step);
    void onScrollDown(int step);
    void onScrollTriggered(int action);

    void onError(QString error_msg);
    void onTranslateReady(const QJsonObject result);
    void onLookupReady(const QJsonObject result);

    void onDialogClose(void);

private:
    void handleNext(int location);
    void handlePrev(int location);

    void drawNext(unsigned int index);
    void drawPrev(unsigned int index);
    void drawFirst(void);

    void start(void);

    void fillBuffer(vector<Index> indexes);

    bool pointBeyondScene(float x, float y);

    void eraseFront(Index index);
    void eraseBack(Index index);
    void erasePages();

private:
    void initConnectors(void);

signals:
    void translate(QString text);
    void lookup(QString text);

    void pageChange(unsigned int index);
};


class DocWidgetHandler : public QObject {
    Q_OBJECT

    using Parametr = QPair<QString, QString>;
    using Parametrs = QList<Parametr>;

    DocWidget* ui;       // не владеет
    DocHandler* handler; // не владеет

    QTreeView* table_of_content; //владеет

    TrWorker trnsl;
    DictWorker dict;

    FindText dialog;

    QMap<QString, QStringList> dict_langs;

    QString from_tr_lang;
    QString to_tr_lang;
    QString from_dict_lang;
    QString to_dict_lang;

    QStandardItemModel* toc;

public:
    DocWidgetHandler(DocWidget* ui, DocHandler* h);
    ~DocWidgetHandler() override;

    DocWidget* getWidget(void) const;

private:
    void scaleMenuConnector(void);
    void translatorMenuConnector(void);
    void pageNavConnector(void);

    void docHandlerConnector(void);
    void contextMenuConnector(void);
    void translatorConnector(void);


    void toolBarConnectors(void);
    void widgetConnectors(void);

signals:
    void translate(const QString text, Parametrs parametrs);
    void lookup(const QString text, Parametrs parametrs);

    void getTrLangs(void);
    void getDictLangs(void);

public slots:
    void onAbsoluteScaleChanged(const QString& value);

    void onZoomIn(void);
    void onZoomOut(void);

    void onTrFromChanged(const QString& lang_name);
    void onTrToChanged(const QString& lang_name);

    void onTranslate(const QString text);
    void onLookup(const QString text);

    void onDictLangsReady(const QJsonArray langs);
    void onError(QString error_msg);

    void onPageChange(unsigned int index);

    void onTOCActivated(const QModelIndex& index);
    void onChangePage(const QString& page);

    void onNextPage();
    void onPrevPage();
    void onLastPage();
    void onFirstPage();
    void onFullScreen();

    void onFind(const QString text);
    void onFindDialogShow(void);
    void onFindDialogClose(void);

private slots:
    void onReload(void);
};


class MainHandler : public QObject {
    Q_OBJECT

    using VOID_SLOT = void (MainHandler::*)(void);
    using BOOL_SLOT = void (MainHandler::*)(bool);
    using INT_SLOT = void (MainHandler::*)(int);

    using VOID_SLOTS = vector<VOID_SLOT>;
    using INT_SLOTS = vector<INT_SLOT>;
    using BOOL_SLOTS = vector<BOOL_SLOT>;

    MainWindow* ui; // не владеет
    vector<DocWidget*> widgets;
    vector<DocHandler*> doc_handlers;
    vector<DocWidgetHandler*> handlers;
    vector<BaseDocument*> documents;
    unsigned int amount = 0;

    Many2One<QAction, MainHandler> file_menu;
    Many2One<QAction, MainHandler> view_menu;
    Many2One<QAction, MainHandler> tools_menu;
    Many2One<QAction, MainHandler> help_menu;

    One2One<QTabWidget, MainHandler> central;


public:
    MainHandler(MainWindow* window);
    ~MainHandler();

    void open(QList<QUrl> files);
    void close(int index);

    vector<void (MainHandler::*)(void)> getMenuHandlers(void);

private:
    void createDocWidget(QUrl path);

    void initConnectors(void);

    const VOID_SLOTS fileMenuSlots(void) const;
    const VOID_SLOTS viewMenuSlots(void) const;
    const VOID_SLOTS toolMenuSlots(void) const;
    const VOID_SLOTS helpMenuSlots(void) const;
    const INT_SLOTS centralSlots(void) const;

public slots:
    void onOpen(void);
    void onPrint(void);
    void onProperty(void);
    void onClose(void);
    void onQuit(void);

    void onZoomIn(void);
    void onZoomOut(void);
    void onFirstPage(void);
    void onLastPage(void);
    void onNextPage(void);
    void onPrevPage(void);
    void onFullScreen(void);

    void onHighlight(void);
    void onUnderline(void);
    void onDashed(void);
    void onTranslator(void);

    void onHelp(void);
    void onAbout(void);

    void onTabClicked(int);
    void onTabChanged(int);
    void onTabClosed(int);
};
#endif // HANDLERS_H
