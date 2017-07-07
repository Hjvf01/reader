#include "ui.h"


DocWidget::DocWidget(QWidget *parent) : QMainWindow(parent) {
    addToolBar(tool_bar);
    //setCentralWidget(view);

    for(QAction* act: context_menu)
        addAction(act);
    unsigned int len = context_menu.size();
    for(unsigned int i = 0; i < len; i++) {
        if(context_menu[i]->iconText() == QString())
            context_menu[i]->setSeparator(true);
        else
            context_menu[i]->setShortcut(context_menu_short[i]);
        context_menu[i]->setIcon(context_menu_icons[i]);
    }
    setContextMenuPolicy(Qt::ActionsContextMenu);
}

//DocView* DocWidget::getView() const { return view; }
DocToolBar* DocWidget::getToolBar() const { return tool_bar; }

QAction* DocWidget::getFindAction() const { return context_menu[0]; }
QAction* DocWidget::getFirstPage()  const { return context_menu[2]; }
QAction* DocWidget::getPrevPage()   const { return context_menu[3]; }
QAction* DocWidget::getNextPage()   const { return context_menu[4]; }
QAction* DocWidget::getLastPage()   const { return context_menu[5]; }
QAction* DocWidget::getZoomIn()     const { return context_menu[7]; }
QAction* DocWidget::getZoomOut()    const { return context_menu[8]; }
QAction* DocWidget::getFullScreen() const { return context_menu[10]; }


void DocWidget::setLeftDock(QWidget *widget) {
    left_dock->setWidget(widget);
    addDockWidget(Qt::LeftDockWidgetArea, left_dock);
}


DocWidget::~DocWidget() {
    for(auto act: context_menu) delete act;
    //delete view;
    delete tool_bar;
    delete left_dock;
}
