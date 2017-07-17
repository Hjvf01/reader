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

DocToolBar* DocWidget::getToolBar() const { return tool_bar; }

unsigned int DocWidget::getContextMenuSize() const {
    return 8;
}

const vector<QAction*> DocWidget::getContextMenu() const {
    return {
        context_menu[0],
        context_menu[2],
        context_menu[3],
        context_menu[4],
        context_menu[5],
        context_menu[7],
        context_menu[8],
        context_menu[10]
    };
}


void DocWidget::setLeftDock(QWidget *widget) {
    //left_dock->setWidget(widget);
    //addDockWidget(Qt::LeftDockWidgetArea, left_dock);
}


DocWidget::~DocWidget() {
    for(auto act: context_menu) delete act;
    //delete view;
    delete tool_bar;
}
