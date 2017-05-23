#ifndef HANDLERS_H
#define HANDLERS_H

#include <deque>
using std::deque;

#include <QtWidgets/QGraphicsPixmapItem>

#include "../ui/ui.h"
#include "../model/models.h"
#include "../network/network.h"
#include "../ui/dialogs.h"


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
    double dpix; double dpiy;
    int location;
    double scale_factor;
    TrDialog tr_dialog;
    bool dialog_shown = false;

public:
    DocHandler(DocView* widget, BaseDocument* doc);
    ~DocHandler();

    int getCurrentPage(void) const;
    int getLocation(void) const;

    DocView* getView(void) const;
    BaseDocument* getDoc(void) const;
    deque<Page*> getPages() const { return pages; }

    void resize(int new_value);

    void goTo(unsigned int index);

public slots:
    void onDoubleClick(QPointF point);

    void onScrollUp(int step);
    void onScrollDown(int step);

    void onError(QString error_msg);
    void onTranslateReady(const QJsonObject result);
    void onLookupReady(const QJsonObject result);

    void onDialogClose(void);

private:
    void drawNext(unsigned int index);
    void drawPrev(unsigned int index);
    void drawFirst(void);

    void start(void);

    void fillBuffer(vector<Index> indexes);
    vector<Index> getIndexes(void);

    bool pointBeyondScene(float x, float y);

    void eraseFront(Index index);
    void eraseBack(Index index);

signals:
    void translate(QString text);
    void lookup(QString text);

    void pageChange(unsigned int index);
};


void docViewConnector(DocView *sender, DocHandler *receiver);


class DocWidgetHandler : public QObject {
    Q_OBJECT

    using Parametr = QPair<QString, QString>;
    using Parametrs = QList<Parametr>;

    DocWidget* ui;       // не владеет
    DocHandler* handler; // не владеет

    QTreeView* table_of_content; //владеет

    TrWorker trnsl;
    DictWorker dict;

    QMap<QString, QStringList> dict_langs;

    QString from_tr_lang;
    QString to_tr_lang;
    QString from_dict_lang;
    QString to_dict_lang;

public:
    DocWidgetHandler(DocWidget* ui, DocHandler* h);
    ~DocWidgetHandler() override;

    DocWidget* getWidget(void) const;

private:
    void webConnector(void);
    void uiConnector(void);
    void connector(void);
    void contextMenuConnector(void);

signals:
    void translate(const QString text, Parametrs parametrs);
    void lookup(const QString text, Parametrs parametrs);

    void getTrLangs(void);
    void getDictLangs(void);

public slots:
    void onAbsoluteScaleChanged(const QString& value);

    void onTrFromChanged(const QString& lang_name);
    void onTrToChanged(const QString& lang_name);

    void onTranslate(const QString text);
    void onLookup(const QString text);

    void onZoomIn(void);
    void onZoomOut(void);

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
};


class MainHandler : public QObject {
    Q_OBJECT

    using Parametr = QPair<QString, QString>;

    MainWindow* ui; // не владеет
    vector<DocWidget*> widgets;
    vector<DocHandler*> doc_handlers;
    vector<DocWidgetHandler*> handlers;
    vector<BaseDocument*> documents;
    unsigned int amount = 0;

    TrWorker trnsl;
    DictWorker dict;

public:
    MainHandler(MainWindow* window);
    ~MainHandler();

    void open(QList<QUrl> files);
    void close(int index);

private:
    void createDocWidget(QUrl path);

public slots:
    void onOpen(void);
    void onPrint(void);
    void onProperty(void);
    void onClose(void);

    void onZoomIn(void);
    void onZoomOut(void);
    void onNextPage(void);
    void onPrevPage(void);
    void onFullScreen(void);

    void onHelp(void);
    void onAbout(void);

    void onTabClicked(int);
    void onTabChanged(int);
    void onTabClosed(int);
};
#endif // HANDLERS_H
