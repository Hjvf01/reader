#include "ui.h"


DocWidget::DocWidget(QWidget *parent) : QMainWindow(parent) {
    addToolBar(tool_bar);
    setCentralWidget(view);
}

DocView* DocWidget::getView() const { return view; }
DocToolBar* DocWidget::getToolBar() const { return tool_bar; }

void DocWidget::setLeftDock(QWidget *widget) {
    left_dock->setWidget(widget);
    addDockWidget(Qt::LeftDockWidgetArea, left_dock);
}


void DocWidget::mousePressEvent(QMouseEvent *event) {
    if(event->button() == Qt::RightButton) {
        DocContextMenu menu;
        emit menuConnectSignal(&menu);
        auto res = menu.exec(event->globalPos());
        qDebug() << res;
    }
}


DocWidget::~DocWidget() {
    delete view;
    delete tool_bar;
    delete left_dock;
}
