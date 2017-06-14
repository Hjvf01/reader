#include <cassert>

#include "ui.h"


MenuBar::MenuBar(QMainWindow* parent) : QMenuBar(parent) {
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
    help_group_actions[0]->setShortcut(help_group_short[0]);

    assert(file_group_actions.size() == file_group_icons.size());
    assert(view_group_actions.size() == view_group_icons.size());

    len = file_group_actions.size();
    for(int i = 0; i < len; i++) {
        file_group_actions[i]->setIcon(file_group_icons[i]);
    }
    len = view_group_actions.size();
    for(int i = 0; i < len; i++)
        view_group_actions[i]->setIcon(view_group_icons[i]);
}


vector<QAction*> MenuBar::getFileGroup() const { return file_group_actions; }
vector<QAction*> MenuBar::getViewGroup() const { return view_group_actions; }
vector<QAction*> MenuBar::getToolGroup() const { return tools_group_actions; }
vector<QAction*> MenuBar::getHelpGroup() const { return help_group_actions; }

const vector<void (QAction::*)(bool)>MenuBar::getFileGroupSignals() const {
    return vector<void (QAction::*)(bool)>(
        file_group_actions.size(),
        &QAction::triggered
    );
}

const vector<void (QAction::*)(bool)>MenuBar::getViewGroupSignals() const {
    return vector<void (QAction::*)(bool)>(
        view_group_actions.size(),
        &QAction::triggered
    );
}

const vector<void (QAction::*)(bool)>MenuBar::getToolGroupSignals() const {
    return vector<void (QAction::*)(bool)>(
        tools_group_actions.size(),
        &QAction::triggered
    );
}

const vector<void (QAction::*)(bool)>MenuBar::getHelpGroupSignals() const {
    return vector<void (QAction::*)(bool)>(
        help_group_actions.size(),
        &QAction::triggered
    );
}

MenuBar::~MenuBar() {
    for(QMenu* menu: menu_groups) delete menu;
    for(QAction* act: file_group_actions) delete act;
    for(QAction* act: view_group_actions) delete act;
    for(QAction* act: help_group_actions) delete act;
    for(QAction* act: tools_group_actions) delete act;
}
