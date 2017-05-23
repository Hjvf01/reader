#include "cassert"

#include "ui.h"


DocContextMenu::DocContextMenu() {
    for(auto a: acts) addAction(a);

    assert(acts.size() == sh.size());

    auto len = acts.size();
    for(unsigned int i = 0; i < len; i++)
        acts[i]->setShortcut(sh[i]);
}


DocContextMenu::~DocContextMenu() {
    for(auto a: acts) delete a;
}
