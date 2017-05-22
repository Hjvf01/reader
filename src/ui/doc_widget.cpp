#include "ui.h"


DocWidget::DocWidget(QWidget *parent) : QMainWindow(parent) {
    addToolBar(tool_bar);
    setCentralWidget(view);
}

DocView* DocWidget::getView() const { return view; }
DocToolBar* DocWidget::getToolBar() const { return tool_bar; }

DocWidget::~DocWidget() {
    delete view;
    delete tool_bar;
}
