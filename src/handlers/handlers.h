#ifndef HANDLERS_H
#define HANDLERS_H

#include <memory>
using std::shared_ptr;
using std::make_shared;
#include <deque>
using std::deque;
#include <string>
using std::string;

#include "../ui/ui.h"
#include "../model/models.h"
#include "../network/network.h"
#include "../ui/dialogs.h"
#include "../connectors/connectors.h"


enum class DoubleClickMode {
    hightlight = 0,
    underline  = 1,
    dashed     = 2,
    translate  = 3,
    clipboard  = 4
};


using DocPtr = shared_ptr<BaseDocument>;
class SceneHandler : public QObject {
    Q_OBJECT

    static DoubleClickMode dbclick_mode = DoubleClickMode::translate;


    string lang_from;
    string lang_to;

    DocScene* scene;
    DocPtr document;
    TrDialog dialog;
    bool dialog_shown;

    One2One<DocScene, SceneHandler> connector;

public:
    SceneHandler(DocPtr doc);
    ~SceneHandler() override;

    DocScene* getScene(void) const;

    static void setDoubleClickMode(const DoubleClickMode& mode);

    void setLangFrom(const QString& lang);
    void setLangTo(const QString& lang);

signals:
    void translate(const QString&);
    void lookup(const QString&);

public slots:
    void onDoubleClick(const QPointF& point);
    void onError(const QString& error_msg);
    void onTranslateReady(const QJsonObject& result);
    void onLookupReady(const QJsonObject& result);

private:
    bool pointBeyondScene(const QPointF& point);
    void initConnectors(void);
    void emitter(const QString& text, const QRectF& box);
};


class DocHandler : public QObject {
    Q_OBJECT

    using Index = unsigned int;
    using PagePtr = PageView*;

    static const Index buf_size = 3;
    DocView* ui;
    Index current;
    deque<PagePtr> pages;
    DocPtr document;
    int location;
    double scale_factor;
    SceneHandler* handler;

    One2One<DocView, DocHandler>* scrolling_connector;
    One2One<ScrollBar, DocHandler>* scroll_bar_connector;

public:
    DocHandler(DocPtr doc);
    ~DocHandler() override;

    unsigned int getCurrentPage(void) const;
    int getLocation(void) const;

    DocView* getView(void) const;

    void resize(int new_value);

    vector<Index> getIndexes(void);

    void goToNext(void);
    void goToPrev(void);
    void goTo(unsigned int index);

public slots:
    void onScrollUp(int step);
    void onScrollDown(int step);
    void onScrollTriggered(int action);

private:
    void handleNext(int location);
    void handlePrev(int location);

    void drawNext(unsigned int index);
    void drawPrev(unsigned int index);
    void drawFirst(void);

    void start(void);

    void fillBuffer(const vector<Index>& indexes);

    void eraseFront(Index index);
    void eraseBack(Index index);
    void erasePages();

    void initConnectors(void);
signals:
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
    void translate(const QString& text, const Parametrs& parametrs);
    void lookup(const QString& text, const Parametrs& parametrs);

    void getTrLangs(void);
    void getDictLangs(void);

public slots:
    void onAbsoluteScaleChanged(const QString& value);

    void onZoomIn(void);
    void onZoomOut(void);

    void onTrFromChanged(const QString& lang_name);
    void onTrToChanged(const QString& lang_name);

    void onTranslate(const QString& text);
    void onLookup(const QString& text);

    void onDictLangsReady(const QJsonArray& langs);
    void onError(const QString& error_msg);

    void onPageChange(unsigned int index);

    void onTOCActivated(const QModelIndex& index);
    void onChangePage(const QString& page);

    void onNextPage(bool);
    void onPrevPage(bool);
    void onLastPage(bool);
    void onFirstPage(bool);
    void onFullScreen(bool);

    void onFind(const QString& text);
    void onFindDialogShow(void);
    void onFindDialogClose(void);

    void onReload(bool);
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

    const BOOL_SLOTS fileMenuSlots(void) const;
    const BOOL_SLOTS viewMenuSlots(void) const;
    const BOOL_SLOTS toolMenuSlots(void) const;
    const BOOL_SLOTS helpMenuSlots(void) const;
    const INT_SLOTS centralSlots(void) const;

public slots:
    void onOpen(bool);
    void onPrint(bool);
    void onProperty(bool);
    void onClose(bool);
    void onQuit(bool);

    void onZoomIn(bool);
    void onZoomOut(bool);
    void onFirstPage(bool);
    void onLastPage(bool);
    void onNextPage(bool);
    void onPrevPage(bool);
    void onFullScreen(bool);

    void onHighlight(bool);
    void onUnderline(bool);
    void onDashed(bool);
    void onTranslator(bool);

    void onHelp(bool);
    void onAbout(bool);

    void onTabClicked(int);
    void onTabChanged(int);
    void onTabClosed(int);
};
#endif // HANDLERS_H
