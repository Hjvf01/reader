#include "ui.h"


DocumentMenu::DocumentMenu(QWidget *parent) : QDockWidget(parent) {
    setWidget(tab_widget);
    tab_widget->addTab(toc_view, QIcon(":/book.png"), "");
    tab_widget->addTab(minimap_view, QIcon(":/view.png"), "");
    tab_widget->addTab(bookmark_view, QIcon(":/bookmark.png"), "");
    tab_widget->setMovable(false);
    tab_widget->setTabPosition(QTabWidget::TabPosition::West);
    setFeatures(QDockWidget::NoDockWidgetFeatures);
}

DocumentMenu::~DocumentMenu() {}


QTreeView* DocumentMenu::getTocView() const { return toc_view; }
QGraphicsView* DocumentMenu::getMinimap() const { return minimap_view; }
QListView* DocumentMenu::getBookmarksView() const { return bookmark_view; }
QTabWidget* DocumentMenu::getTabWidget() const { return tab_widget; }
