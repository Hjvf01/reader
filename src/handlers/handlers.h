#ifndef HANDLERS_H
#define HANDLERS_H

#include <memory>
using std::shared_ptr;
using std::make_shared;
#include <deque>
using std::deque;
#include <string>
using std::string;
#include <algorithm>
using std::move;
using std::forward;


#include "../ui/ui.h"
#include "../model/models.h"
#include "../network/network.h"
#include "../ui/dialogs.h"
#include "../connectors/connectors.h"
#include "../network_handlers/network_handlers.h"



enum class SelectMode {
    hightlight = 0,
    underline  = 1,
    dashed     = 2,
    translate  = 3,
    clipboard  = 4
};




using DocPtr = shared_ptr<BaseDocument>;
class SceneHandler : public QObject {
    Q_OBJECT

    static SelectMode select_mode;

    QString lang_from;
    QString lang_to;

    DocScene* scene;
    DocPtr document;
    bool dialog_shown = false;

    TrDialog dialog;
    YandexWorker translator;
    YandexHandler network_handler;

    One2One<DocScene, SceneHandler> connector;
    One2One<SceneHandler, YandexWorker> scene2network;
    One2One<YandexWorker, YandexHandler> network2handler;
    One2One<YandexHandler, SceneHandler> handler2this;

public:
    SceneHandler(const DocPtr& doc);
    ~SceneHandler() override;

    DocScene* getScene(void) const;

    static void setDoubleClickMode(const SelectMode& mode);

    void setLangFrom(const QString& lang);
    void setLangTo(const QString& lang);

signals:
    void translate(const Parameters&);
    void lookup(const Parameters&);
    void drawFillRect(const QRectF&);
    void drawSolidLine(const QRectF&);
    void drawDashedLine(const QRectF&);
    void copyToClipboard(const QString&);

private slots:
    void onDoubleClick(const QPointF& point);
    void onError(const QString& error_msg);
    void onTranslateReady(const QString& result);
    void onLookupReady(const QString& result);
    void onDialogClose(void);

private:
    bool pointBeyondScene(const QPointF& point) const;
    void initConnectors(void);
    void emitter(const QRectF& box, const QString& text);
};




using PagePtr = PageView*;
class DocHandler : public QObject {
    Q_OBJECT

    static const unsigned int buf_size = 3;

    DocView* ui;
    Index current;
    deque<PagePtr> pages;
    DocPtr document;
    int location;
    double scale_factor;
    SceneHandler* handler;

    One2One<DocView, DocHandler> scrolling_connector;
    One2One<ScrollBar, DocHandler> scroll_bar_connector;

public:
    DocHandler(const DocPtr& doc);
    DocHandler(const DocPtr& doc, Index index);
    ~DocHandler() override;

    unsigned int getCurrentPage(void) const;
    int getLocation(void) const;
    DocView* getView(void) const;
    SceneHandler* getSceneHandler(void) const;

    void resize(int new_value);

    vector<Index> getIndexes(void) const;

    void goToNext(void);
    void goToPrev(void);
    void goTo(unsigned int index);

private slots:
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
    void centerOnCurrentPage(void) const;

signals:
    void pageChange(unsigned int index);
};




class MinimapHandler : public QObject {
    Q_OBJECT

    static const Size BUF_SIZE = 5;


    Index current;
    DocPtr document;
    DocView* ui;
    DocScene* scene;
    int location = 0;

    deque<PageView*> pages;

    One2One<DocView, MinimapHandler> view2this;

public:
    MinimapHandler(const DocPtr& doc);
    MinimapHandler(const DocPtr& doc, Index index);
    ~MinimapHandler() override;

    DocView* getMinimap(void) const;
    Index getCurrent(void) const;
    Indexes getIndexes(void) const;
    DocScene* getScene(void) const;

private slots:
    void onScrollUp(int step);
    void onScrollDown(int step);
    void onScrollTriggered(int action);

private:
    void drawNext(Index index);
    void drawPrev(Index index);

    void eraseFront(Index index);
    void eraseBack(Index index);

    void start(void);

    void initConnectors(void);
};




class DocMenuHandler : public QObject {
    Q_OBJECT

    DocumentMenu* ui;
    MinimapHandler* minimap_h;
    TocModel* toc_model;
    DocPtr document;

public:
    DocMenuHandler(const QUrl& path);
    ~DocMenuHandler() override;

    DocumentMenu* getDocumentMenu(void) const { return ui; }

signals:
    void changePage(const QString& page);

private slots:
    void onActivated(const QModelIndex& item);

private:
    void initConnectors(void);
};




class DocWidgetHandler : public QObject {
    Q_OBJECT

    using Langs = QMap<QString, QList<QString>>;

    DocWidget* ui;
    DocToolBar* tool_bar;
    DocHandler* handler;
    DocMenuHandler* doc_menu_h;
    bool doc_menu_shown = true;

    DocPtr document;

    FindDialog find_dialog;

    Langs dict_langs;
    YandexWorker langs;
    YandexHandler network_handler;

    Many2One<QAction, DocWidgetHandler> context_menu_connector;
    Many2One<QComboBox, DocWidgetHandler> combo_box_connector;
    Many2One<QAction, DocWidgetHandler> tool_bar_connector;

public:
    DocWidgetHandler(const QUrl& path);
    ~DocWidgetHandler() override;

    DocWidget* getWidget(void) const;
    DocHandler* getHandler(void) const;
    BaseDocument* getDocument(void) const;
    DocToolBar* getToolBar(void) const { return tool_bar; }

private:
    void initConnectors(void);

signals:
    void getLangs(void);

private slots:
    void onAbsoluteScaleChanged(const QString& value);

    void onTrFromChanged(const QString& lang_name);
    void onTrToChanged(const QString& lang_name);

    void onDictLangsReady(const Langs& langs);
    void onError(const QString& error_msg);

    void onPageChange(unsigned int index);
    void onPageChange(const QString& page);

    void onZoomIn(bool);
    void onZoomOut(bool);
    void onNextPage(bool);
    void onPrevPage(bool);
    void onLastPage(bool);
    void onFirstPage(bool);
    void onFullScreen(bool);

    void onFind(const QString& text);
    void onFindDialogClose(void);
    void onFindDialogShow(bool);

    void onReload(bool);

    void onDocementMenuShow(bool);
};




using DocWidgetPtr = shared_ptr<DocWidgetHandler>;
class MainHandler : public QObject {
    Q_OBJECT

    MainWindow* ui;
    vector<DocWidgetPtr> documents;

    Many2One<QAction, MainHandler> file_menu;
    Many2One<QAction, MainHandler> view_menu;
    Many2One<QAction, MainHandler> tools_menu;
    Many2One<QAction, MainHandler> help_menu;

    One2One<QTabWidget, MainHandler> central;


public:
    MainHandler();
    ~MainHandler() override;

    void initConnectors(void);

    void open(const QList<QUrl>& files);
    void close(int index);

    MainWindow* getWindow(void) const;

private slots:
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
