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

#include <QtGui/QWheelEvent>
#include <QtGui/QKeyEvent>
#include <QtGui/QDragEnterEvent>

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

    QGraphicsRectItem* hightlight;

public:
    DocScene();
    ~DocScene();

    void setHightLight(QRectF rect);
    QGraphicsRectItem* getHightlight(void) const;

    void eraseHightlight(void);

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;

signals:
    void doubleClick(QPointF point);
};


class ScrollBar : public QScrollBar {
    Q_OBJECT
};


class DocView : public QGraphicsView {
    Q_OBJECT

    DocScene* scene;
    ScrollBar* scroll;
    int scroll_step;

public:
    DocView(QWidget* parent=nullptr);
    ~DocView();

    void initScene(QSize* size);

    QScrollBar* getScroll(void) const;
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

    QAction* zoom_in;
    QAction* zoom_out;
    QComboBox* scale_box;
    const QList<QString> scale_factors = {
        "50", "60", "70", "80", "90", "100",
        "110", "120", "130", "140", "150"
    };
    QLabel* tr_title;
    QLabel* tr_from_title;
    QLabel* tr_to_title;
    QComboBox* tr_from;
    QComboBox* tr_to;

    QLineEdit current_page;
    QLabel page_count;

public:
    DocToolBar(QWidget* parent=nullptr);
    ~DocToolBar() override;

    QComboBox* getScaleBox(void) const;
    QComboBox* getTrFrom(void)   const;
    QComboBox* getTrTo(void)     const;
    QAction* getZoomIn(void)     const;
    QAction* getZoomOut(void)    const;

    QLineEdit* getCurrentPage(void);
    QLabel* getPageLbl(void);

    void setPageNums(const QString current, const QString last);
    void setCurrentPage(const QString num);

signals:
    void setPage(void);
};


class DocContextMenu : public QMenu {
    Q_OBJECT

    using ShortCuts = vector<QKeySequence>;
    using Actions = vector<QAction*>;

    const Actions acts = {
        new QAction("First Page"),
        new QAction("Previous Page"),
        new QAction("Next Page"),
        new QAction("Last Page"),
        new QAction("Zoom in"),
        new QAction("Zoom out"),
        new QAction("Full Screen")
    };

    const ShortCuts sh = {
        QKeySequence(Qt::Key_Home),
        QKeySequence(Qt::CTRL + Qt::Key_Left),
        QKeySequence(Qt::CTRL + Qt::Key_Right),
        QKeySequence(Qt::Key_End),
        QKeySequence(Qt::CTRL + Qt::Key_Equal),
        QKeySequence(Qt::CTRL + Qt::Key_Minus),
        QKeySequence(Qt::CTRL + Qt::Key_F11)
    };

public:
    DocContextMenu();
    ~DocContextMenu() override;

    QAction* getFirstPage() const { return acts[0]; }
    QAction* getPrevPage() const { return acts[1]; }
    QAction* getNextPage() const { return acts[2]; }
    QAction* getLastPage() const { return acts[3]; }
    QAction* getZoomIn() const { return acts[4]; }
    QAction* getZoomOut() const { return acts[5]; }
    QAction* getFullScreen() const { return acts[6]; }
};


class DocWidget : public QMainWindow {
    Q_OBJECT

    DocView* view = new DocView;
    DocToolBar* tool_bar = new DocToolBar;
    QDockWidget* left_dock = new QDockWidget;

public:
    DocWidget(QWidget* parent=nullptr);
    ~DocWidget() override;

    DocView* getView(void) const;
    DocToolBar* getToolBar(void) const;

    void setLeftDock(QWidget* widget);

protected:
    void mousePressEvent(QMouseEvent* event) override;

signals:
    void menuConnectSignal(DocContextMenu* menu);
};


class MenuBar : public QMenuBar {
    Q_OBJECT

    using ShortCuts = vector<QKeySequence>;
    using MenuList = vector<QMenu*>;
    using Actions = vector<QAction*>;

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

    QAction* getHelp(void)  const;
    QAction* getAbout(void) const;
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
