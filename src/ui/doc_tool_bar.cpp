#include "ui.h"


DocToolBar::DocToolBar(QWidget *parent) : QToolBar(parent) {
    initScaleMenu();
    addSeparator();

    initTranslatorMenu();
    addSeparator();

    initPageNavMenu();

    setMovable(false);
}

DocToolBar::~DocToolBar() {
    delete zoom_in;
    delete zoom_out;
    delete scale_box;

    delete fst_page;
    delete prev_page;
    delete next_page;
    delete last_page;
    delete find;

    delete tr_from;
    delete tr_to;
}

void DocToolBar::setPageNums(const QString current, const QString last) {
    current_page.setText(current);
    page_count.setText("\t<html><strong>" + last + "</strong></html>");
    current_page.setValidator(new QIntValidator(1, last.toInt() - 1));
}

void DocToolBar::setCurrentPage(const QString num) {
    current_page.setText(num);
}


QComboBox* DocToolBar::getScaleBox() const { return scale_box; }
QComboBox* DocToolBar::getTrFrom()   const { return tr_from; }
QComboBox* DocToolBar::getTrTo()     const { return tr_to; }

QAction* DocToolBar::getZoomIn()  const { return zoom_in; }
QAction* DocToolBar::getZoomOut() const { return zoom_out; }

QAction* DocToolBar::getReload() const { return reload; }

QAction* DocToolBar::getFstPage() const { return fst_page; }
QAction* DocToolBar::getPrevPage() const { return prev_page; }
QAction* DocToolBar::getNextPage() const { return next_page; }
QAction* DocToolBar::getLastPage() const { return last_page; }
QAction* DocToolBar::getFind() const { return find; }

QLineEdit* DocToolBar::getCurrentPage() { return &current_page; }
QLabel* DocToolBar::getPageLbl() { return &page_count; }


void DocToolBar::initScaleMenu() {
    zoom_in = new QAction;
    zoom_in->setIcon(QIcon(":/zoom_in.png"));
    zoom_out = new QAction;
    zoom_out->setIcon(QIcon(":/zoom_out.png"));

    scale_box = new QComboBox;
    scale_box->addItems(scale_factors);
    scale_box->setCurrentIndex(5);

    addAction(zoom_out);
    addWidget(scale_box);
    addAction(zoom_in);
}


void DocToolBar::initTranslatorMenu() {
    QIcon tr_icon(":/translator.png");
    QIcon sep_icon(":/tr_separator.png");
    QIcon reload_icon(":/reload.png");

    reload = new QAction;
    reload->setIcon(reload_icon);

    tr_title.setPixmap(tr_icon.pixmap(ICON_SIZE, ICON_SIZE));
    tr_title.setAlignment(Qt::AlignCenter);
    tr_title.setFixedWidth(ICON_SIZE * 2);
    sep.setPixmap(sep_icon.pixmap(ICON_SIZE, ICON_SIZE));
    sep.setFixedWidth(ICON_SIZE * 2);
    sep.setAlignment(Qt::AlignCenter);

    tr_from = new QComboBox;
    tr_to = new QComboBox;

    addWidget(&tr_title);
    addWidget(tr_from);
    addWidget(&sep);
    addWidget(tr_to);
    addAction(reload);
}


void DocToolBar::initPageNavMenu() {
    QIcon title_icon(":/file.png"),
          fst_page_icon(":/first_page.png"),
          prev_page_icon(":/prev_page.png"),
          next_page_icon(":/next_page.png"),
          last_page_icon(":/last_page.png"),
          find_icon(":/find.png");

    fst_page = new QAction;
    prev_page = new QAction;
    next_page = new QAction;
    last_page = new QAction;
    find = new QAction;

    fst_page->setIcon(fst_page_icon);
    prev_page->setIcon(prev_page_icon);
    next_page->setIcon(next_page_icon);
    last_page->setIcon(last_page_icon);
    find->setIcon(find_icon);

    page_title.setPixmap(title_icon.pixmap(ICON_SIZE, ICON_SIZE));
    page_title.setAlignment(Qt::AlignCenter);
    page_title.setFixedWidth(ICON_SIZE * 2);


    current_page.setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    addWidget(&page_title);
    addAction(fst_page);
    addAction(prev_page);
    addWidget(&current_page);
    addWidget(&page_count);
    addAction(next_page);
    addAction(last_page);
    addAction(find);
}
