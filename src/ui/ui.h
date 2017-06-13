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


class DocScene : public QGraphicsScene {
    Q_OBJECT

    QGraphicsRectItem* select_box = nullptr;
    vector<QGraphicsRectItem*> select_boxes;

public:
    DocScene();
    ~DocScene();

    void setHightLight(QRectF rect);
    void setHightLights(vector<QRectF> rects);
    QGraphicsRectItem* getHightlight(void) const;

    void eraseHightlight(void);
    void eraseHightlights(void);

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;

signals:
    void doubleClick(QPointF point);
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

    DocScene* scene;
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

public:
    DocToolBar(QWidget* parent=nullptr);
    ~DocToolBar() override;

    QComboBox* getScaleBox(void) const;
    QComboBox* getTrFrom(void)   const;
    QComboBox* getTrTo(void)     const;

    QAction* getZoomIn(void)  const;
    QAction* getZoomOut(void) const;

    QAction* getReload(void) const;

    QAction* getFstPage(void)  const;
    QAction* getPrevPage(void) const;
    QAction* getNextPage(void) const;
    QAction* getLastPage(void) const;
    QAction* getFind(void)     const;

    QLineEdit* getCurrentPage(void);
    QLabel* getPageLbl(void);

    void setPageNums(const QString current, const QString last);
    void setCurrentPage(const QString num);

private:
    void initScaleMenu(void);
    void initTranslatorMenu(void);
    void initPageNavMenu(void);

signals:
    void setPage(void);
};


class DocWidget : public QMainWindow {
    Q_OBJECT

    using Actions = vector<QAction*>;
    using ShortCuts = vector<QKeySequence>;
    using Icons = vector<QIcon>;

    DocView* view = new DocView;
    DocToolBar* tool_bar = new DocToolBar;
    QDockWidget* left_dock = new QDockWidget;

    const Actions context_menu = {
        new QAction("Find"),                //0
        new QAction(),                      //1
        new QAction("First Page"),          //2
        new QAction("Previous Page"),       //3
        new QAction("Next Page"),           //4
        new QAction("Last Page"),           //5
        new QAction(),                      //6
        new QAction("Zoom in"),             //7
        new QAction("Zoom out"),            //8
        new QAction(),                      //9
        new QAction("Full Screen")          //10
    };
    const ShortCuts context_menu_short = {
        QKeySequence(Qt::CTRL + Qt::Key_F),
        QKeySequence(),
        QKeySequence(Qt::Key_Home),
        QKeySequence(Qt::CTRL + Qt::Key_Left),
        QKeySequence(Qt::CTRL + Qt::Key_Right),
        QKeySequence(Qt::Key_End),
        QKeySequence(),
        QKeySequence(Qt::CTRL + Qt::Key_Equal),
        QKeySequence(Qt::CTRL + Qt::Key_Minus),
        QKeySequence(),
        QKeySequence(Qt::CTRL + Qt::Key_F11)
    };
    const Icons context_menu_icons = {
        QIcon(":/find.png"),
        QIcon(),
        QIcon(":/first_page.png"),
        QIcon(":/prev_page.png"),
        QIcon(":/next_page.png"),
        QIcon(":/last_page.png"),
        QIcon(),
        QIcon(":/zoom_in.png"),
        QIcon(":/zoom_out.png"),
        QIcon(),
        QIcon(":/fullscreen.png"),
    };

public:
    DocWidget(QWidget* parent=nullptr);
    ~DocWidget() override;

    DocView* getView(void) const;
    DocToolBar* getToolBar(void) const;

    QAction* getFindAction(void) const;
    QAction* getFirstPage(void) const;
    QAction* getPrevPage(void) const;
    QAction* getNextPage(void) const;
    QAction* getLastPage(void) const;
    QAction* getZoomIn(void) const;
    QAction* getZoomOut(void) const;
    QAction* getFullScreen(void) const;

    void setLeftDock(QWidget* widget);
};


class MenuBar : public QMenuBar {
    Q_OBJECT

    using ShortCuts = vector<QKeySequence>;
    using MenuList = vector<QMenu*>;
    using Actions = vector<QAction*>;
    using Icons = vector<QIcon>;

    const MenuList menu_groups = {
        new QMenu("File"),
        new QMenu("View"),
        new QMenu("Tools"),
        new QMenu("Help"),
    };

    const Actions file_group_actions = {
        new QAction("Open"),
        new QAction("Print"),
        new QAction("Property"),
        new QAction("Close"),
        new QAction("Quit")
    };
    const Actions view_group_actions = {
        new QAction("Zoom-in"),
        new QAction("Zoom-out"),
        new QAction("First Page"),
        new QAction("Next Page"),
        new QAction("Previous Page"),
        new QAction("Last Page"),
        new QAction("Full Screen")
    };
    const Actions tools_group_actions = {
        new QAction("Hightlight Text"),
        new QAction("Underline Text"),
        new QAction("Dashed underline Text"),
        new QAction("Translator"),
    };
    const Actions help_group_actions = {
        new QAction("Help"),
        new QAction("About")
    };

    const ShortCuts file_group_short = {
        QKeySequence("Ctrl+O"),
        QKeySequence("Ctrl+P"),
        QKeySequence("Alt+Return"),
        QKeySequence("Ctrl+W"),
        QKeySequence("Ctrl+Q"),
    };
    const ShortCuts view_group_short = {
        QKeySequence(Qt::CTRL + Qt::Key_Equal),
        QKeySequence(Qt::CTRL + Qt::Key_Minus),
        QKeySequence(Qt::Key_Home),
        QKeySequence(Qt::CTRL + Qt::Key_Left),
        QKeySequence(Qt::CTRL + Qt::Key_Right),
        QKeySequence(Qt::Key_End),
        QKeySequence(Qt::ALT + Qt::Key_F11)
    };
    const ShortCuts help_group_short = {
        QKeySequence("Ctrl+H")
    };

    const Icons file_group_icons = {
        QIcon(":/file.png"),
        QIcon(":/print.png"),
        QIcon(":/property.png"),
        QIcon(":/close.png"),
        QIcon(":/quit.png"),
    };
    const Icons view_group_icons = {
        QIcon(":/zoom_in.png"),
        QIcon(":/zoom_out.png"),
        QIcon(":/first_page.png"),
        QIcon(":/prev_page.png"),
        QIcon(":/next_page.png"),
        QIcon(":/last_page.png"),
        QIcon(":/fullscreen.png"),
    };

public:
    MenuBar(QMainWindow* parent);
    ~MenuBar() override;

    QAction* getOpen(void)     const;
    QAction* getPrint(void)    const;
    QAction* getProperty(void) const;
    QAction* getClose(void)    const;
    QAction* getQuit(void)     const;

    QAction* getZoomIn(void)     const;
    QAction* getZoomOut(void)    const;
    QAction* getNextPage(void)   const;
    QAction* getPrevPage(void)   const;
    QAction* getFullScreen(void) const;

    QAction* getHightlight(void)    const;
    QAction* getUnderlineText(void) const;
    QAction* getDashedText(void)    const;
    QAction* getTranslator(void)    const;

    QAction* getHelp(void)  const;
    QAction* getAbout(void) const;

    vector<QAction*> getActions(void) const;

    vector<QAction*> getFileGroup(void) const;
    const vector<void (QAction::*)(bool)> getFileGroupTriger(void) const;

    vector<QAction*> getViewGroup(void) const;
    const vector<void (QAction::*)(bool)> getViewGroupTriger(void) const;
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
