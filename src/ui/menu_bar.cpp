#include <cassert>

#include "ui.h"


MenuBar::MenuBar(QMainWindow* parent) : QMenuBar(parent) {
    const ShortCuts file_group_short = {
        QKeySequence("Ctrl+O"), QKeySequence("Ctrl+P"),
        QKeySequence("Alt+Return"), QKeySequence("Ctrl+W"),
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
        QKeySequence("Ctrl+H"), QKeySequence()
    };

    assert(file_group_actions.size() == file_group_short.size());
    assert(view_group_actions.size() == view_group_short.size());

    for(QMenu* menu: menu_groups)
        addMenu(menu);

            /* file group */
    for(QAction* act: file_group_actions)
        menu_groups[0]->addAction(act);
            /* view group */
    for(QAction* act: view_group_actions) {
        menu_groups[1]->addAction(act);
        act->setShortcutContext(Qt::WidgetWithChildrenShortcut);
    }
            /* tools group */
    for(QAction* act: tools_group_actions) {
        menu_groups[2]->addAction(act);
    }
            /* help group */
    for(QAction* act: help_group_actions)
        menu_groups[3]->addAction(act);

    menu_groups[0]->insertSeparator(file_group_actions[4]);
    menu_groups[1]->insertSeparator(view_group_actions[2]);
    menu_groups[1]->insertSeparator(view_group_actions[6]);
    menu_groups[2]->insertSeparator(tools_group_actions[3]);

    int len = file_group_actions.size();
    for(int i = 0; i < len; i++)
        file_group_actions[i]->setShortcut(file_group_short[i]);
    len = view_group_actions.size();
    for(int i = 0; i < len; i++)
        view_group_actions[i]->setShortcut(view_group_short[i]);
    len = help_group_actions.size();
    for(int i = 0; i < len; i++)
        help_group_actions[i]->setShortcut(help_group_short[i]);
}


vector<QAction*> MenuBar::getFileGroup() const { return file_group_actions; }
vector<QAction*> MenuBar::getViewGroup() const { return view_group_actions; }
vector<QAction*> MenuBar::getToolGroup() const { return tools_group_actions; }
vector<QAction*> MenuBar::getHelpGroup() const { return help_group_actions; }

unsigned int MenuBar::fileGroupSize() const {
    return file_group_actions.size();
}
unsigned int MenuBar::viewGroupSize() const {
    return view_group_actions.size();
}

unsigned int MenuBar::toolGroupSize() const {
    return tools_group_actions.size();
}

unsigned int MenuBar::helpGroupSize() const {
    return help_group_actions.size();
}


MenuBar::~MenuBar() {
    for(QMenu* menu: menu_groups) delete menu;
    for(QAction* act: file_group_actions) delete act;
    for(QAction* act: view_group_actions) delete act;
    for(QAction* act: help_group_actions) delete act;
    for(QAction* act: tools_group_actions) delete act;
}
