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
    return context_menu.size() - 3;
}

const vector<QAction*> DocWidget::getContextMenu() const {
    vector<QAction*> context;
    unsigned int len = context_menu.size();
    for(unsigned int i = 0; i < len; i++)
        if(i != 1 || i != 6 || i != 9)
            context.push_back(context_menu[i]);
    return context;
}


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
