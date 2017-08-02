#ifndef UI_H
#define UI_H

#include <vector>
using std::vector;

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QAction>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMenu>
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGraphicsPixmapItem>
#include <QtWidgets/QScrollBar>
#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QListView>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QPushButton>

#include <QtGui/QWheelEvent>
#include <QtGui/QKeyEvent>
#include <QtGui/QDragEnterEvent>
#include <QtGui/QIcon>

#include <QtCore/QDebug>
#include <QtCore/QList>
#include <QtCore/QPair>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QSizeF>
#include <QtCore/QPointF>
#include <QtCore/Qt>



using Index = unsigned int;




class PageView : public QGraphicsPixmapItem {
    Index index;

public:
    PageView(const QPixmap& pix, Index i);
    ~PageView() override;

    Index getIndex(void) const;
    void setIndex(Index _index) { index = _index; }
};




class DocScene : public QGraphicsScene {
    Q_OBJECT

    using DbClick = void (DocScene::*)(const QPointF&);

    QGraphicsRectItem* select_box = nullptr;
    vector<QGraphicsRectItem*> select_boxes;

public:
    DocScene();
    DocScene(const QRectF& scene_rect, QWidget* parent);
    ~DocScene();

    void setHightLight(const QRectF& rect);
    void setHightLights(const vector<QRectF>& rects);
    QGraphicsRectItem* getHightlight(void) const;

    void eraseHightlight(void);
    void eraseHightlights(void);

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;

signals:
    void doubleClick(const QPointF& point);
};




class ScrollBar : public QScrollBar {
    Q_OBJECT

public:
    ScrollBar(QWidget* parent);
    ScrollBar();
    ~ScrollBar() override;
};




class DocView : public QGraphicsView {
    Q_OBJECT

    ScrollBar* scroll;
    int scroll_step = 20;

public:
    DocView(QWidget* parent=nullptr);
    ~DocView();

    void initScene(QSize* size);

    ScrollBar* getScroll(void) const;
    DocScene* getScene(void) const;

signals:
    void scrollUp(int step);
    void scrollDown(int step);

protected:
    void wheelEvent(QWheelEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
};




class DocToolBar : public QToolBar {
    Q_OBJECT

    static const int ICON_SIZE = 20;

    const QList<QString> scale_factors = {
        "50", "60", "70", "80", "90", "100",
        "110", "120", "130", "140", "150"
    };

    QAction* zoom_in;
    QAction* zoom_out;
    QComboBox* scale_box;

    QLabel tr_title;
    QLabel sep;
    QComboBox* tr_from;
    QComboBox* tr_to;
    QAction* reload;

    QLabel page_title;
    QLineEdit current_page;
    QLabel page_count;

    QAction *fst_page,
            *prev_page,
            *next_page,
            *last_page,
            *find;

    QAction* bookmark_btn;

    QIntValidator page_validator;

public:
    DocToolBar(QAction* hide_menu, QWidget* parent=nullptr);
    ~DocToolBar() override;

    QComboBox* getScaleBox(void) const;
    QComboBox* getTrTo(void) const;
    QComboBox* getTrFrom(void) const;

    vector<QComboBox*> getComboBoxes(void) const;
    vector<QAction*> getActions(void) const;

    unsigned int getComboBoxesAmount(void) const;
    unsigned int getActionsAmount(void) const;

    void setPageNums(const QString& current, const QString& last);
    void setCurrentPage(const QString& num);

    QLineEdit* getPageLine(void) { return &current_page; }

    void initValidator(unsigned int i);

private:
    void initScaleMenu(void);
    void initTranslatorMenu(void);
    void initPageNavMenu(void);

signals:
    void setPage(void);
};




class DocumentMenu : public QDockWidget {
    Q_OBJECT

    QTreeView* toc_view = new QTreeView;
    DocView* minimap_view;
    QListView* bookmark_view = new QListView;
    QTabWidget* tab_widget = new QTabWidget;

public:
    DocumentMenu(DocView* minimap, QWidget* parent=nullptr);
    ~DocumentMenu() override;

    QTreeView* getTocView(void) const;
    QGraphicsView* getMinimap(void) const;
    QListView* getBookmarksView(void) const;
    QTabWidget* getTabWidget(void) const;
};




using Actions = vector<QAction*>;
using ShortCuts = vector<QKeySequence>;




class DocWidget : public QMainWindow {
    Q_OBJECT

    const Actions context_menu = {
        new QAction(QIcon(":/find.png"), "Find"),
        new QAction(),
        new QAction(QIcon(":first_page.png"), "First Page"),
        new QAction(QIcon(":/prev_page.png"), "Previous Page"),
        new QAction(QIcon(":/next_page.png"), "Next Page"),
        new QAction(QIcon(":/last_page.png"), "Last Page"),
        new QAction(),
        new QAction(QIcon(":/zoom_in.png"), "Zoom in"),
        new QAction(QIcon(":/zoom_out.png"), "Zoom out"),
        new QAction(),
        new QAction(QIcon(":/fullscreen.png"), "Full Screen")
    };

public:
    DocWidget(QWidget* parent=nullptr);
    ~DocWidget() override;

    DocToolBar* getToolBar(void) const;
    vector<QAction*> getContextMenu(void) const;
    unsigned int getContextMenuSize(void) const;
};




using MenuList = vector<QMenu*>;




class MenuBar : public QMenuBar {
    Q_OBJECT

    const MenuList menu_groups = {
        new QMenu("File"),
        new QMenu("View"),
        new QMenu("Tools"),
        new QMenu("Help"),
    };

    const Actions file_group_actions = {
        new QAction(QIcon(":/file.png"), "Open"),
        new QAction(QIcon(":/print.png"), "Print"),
        new QAction(QIcon(":/property.png"), "Property"),
        new QAction(QIcon(":/close.png"), "Close"),
        new QAction(QIcon(":/quit.png"), "Quit")
    };
    const Actions view_group_actions = {
        new QAction(QIcon(":/zoom_in.png"), "Zoom-in"),
        new QAction(QIcon(":/zoom_out.png"), "Zoom-out"),
        new QAction(QIcon(":/first_page.png"), "First Page"),
        new QAction(QIcon(":/next_page.png"), "Next Page"),
        new QAction(QIcon(":/prev_page.png"), "Previous Page"),
        new QAction(QIcon(":/last_page.png"), "Last Page"),
        new QAction(QIcon(":/fullscreen.png"), "Full Screen")
    };
    const Actions tools_group_actions = {
        new QAction("Hightlight Text"),
        new QAction("Underline Text"),
        new QAction("Dashed underline Text"),
        new QAction("Translator"),
    };
    const Actions help_group_actions = {
        new QAction("Help"), new QAction("About")
    };

public:
    MenuBar(QMainWindow* parent);
    ~MenuBar() override;

    vector<QAction*> getFileGroup(void) const;
    vector<QAction*> getViewGroup(void) const;
    vector<QAction*> getToolGroup(void) const;
    vector<QAction*> getHelpGroup(void) const;

    unsigned int fileGroupSize(void) const;
    unsigned int viewGroupSize(void) const;
    unsigned int toolGroupSize(void) const;
    unsigned int helpGroupSize(void) const;
};




class MainWindow : public QMainWindow {
    Q_OBJECT

    MenuBar* menu = nullptr;
    QTabWidget* central_widget = nullptr;
    QStatusBar* status = nullptr;

public:
    MainWindow();
    ~MainWindow() override;

    QTabWidget* getCentral(void) const;
    MenuBar* getMenu(void) const;

    void statusBarMessage(QString msg);
};
#endif // UI_H
