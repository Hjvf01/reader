#include "ui.h"


DocWidget::DocWidget(QWidget *parent) : QMainWindow(parent) {
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

    for(QAction* act: context_menu)
        addAction(act);
    unsigned int len = context_menu.size();
    for(unsigned int i = 0; i < len; i++) {
        if(context_menu[i]->iconText() == QString())
            context_menu[i]->setSeparator(true);
        else
            context_menu[i]->setShortcut(context_menu_short[i]);
    }
    setContextMenuPolicy(Qt::ActionsContextMenu);
}


unsigned int DocWidget::getContextMenuSize() const { return 8; }


vector<QAction*> DocWidget::getContextMenu() const {
    return {
        context_menu[0], context_menu[2], context_menu[3],
        context_menu[4], context_menu[5], context_menu[7],
        context_menu[8], context_menu[10]
    };
}


DocWidget::~DocWidget() {
    for(auto act: context_menu) delete act;
}
