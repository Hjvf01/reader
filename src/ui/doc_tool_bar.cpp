#include "ui.h"


DocToolBar::DocToolBar(QWidget *parent) : QToolBar(parent) {
    zoom_in = new QAction("+");
    zoom_out = new QAction("-");

    scale_box = new QComboBox;
    scale_box->addItems(scale_factors);
    scale_box->setCurrentIndex(5);

    addAction(zoom_out);
    addWidget(scale_box);
    addAction(zoom_in);
    addSeparator();

    tr_title = new QLabel("<strong>Translator</strong>: ", this);
    tr_from_title = new QLabel("from", this);
    tr_to_title = new QLabel("to");
    tr_from = new QComboBox;
    tr_to = new QComboBox;
    addWidget(tr_title);
    addWidget(tr_from_title);
    addWidget(tr_from);
    addWidget(tr_to_title);
    addWidget(tr_to);
    addSeparator();
}

DocToolBar::~DocToolBar() {
    delete zoom_in;
    delete zoom_out;
    delete scale_box;

    delete tr_title;
    delete tr_from_title;
    delete tr_to_title;
    delete tr_from;
    delete tr_to;
}

void DocToolBar::setPageNums(const QString current, const QString last) {
    current_page.setText(current);
    page_count.setText("\t<html><strong>" + last + "</strong></html>");
    current_page.setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    addWidget(&current_page);
    addWidget(&page_count);
    current_page.setValidator(new QIntValidator(1, last.toInt() - 1));
}

void DocToolBar::setCurrentPage(const QString num) {
    current_page.setText(num);
}


QComboBox* DocToolBar::getScaleBox() const { return scale_box; }
QComboBox* DocToolBar::getTrFrom()   const { return tr_from; }
QComboBox* DocToolBar::getTrTo()     const { return tr_to; }
QAction* DocToolBar::getZoomIn()     const { return zoom_in; }
QAction* DocToolBar::getZoomOut()    const { return zoom_out; }

QLineEdit* DocToolBar::getCurrentPage() { return &current_page; }
QLabel* DocToolBar::getPageLbl() { return &page_count; }
