#include <cassert>

#include <QtWidgets/QAbstractSlider>

#include "handlers.h"


DocHandler::DocHandler(DocView* ui, BaseDocument* doc) : QObject() {
    /*
        1. Размер буфера страниц
        2. Представление этого обработчика
        3. текущая страница
        4. Дек, содержащий страницы
        5. текущее расположени верха виджета относительно начала сцены
        6. Масштаб
    */
    this->ui = ui;
    current = 0;
    document = doc;
    location = 0;
    scale_factor = 1.0;
    pages.resize(buf_size);

    initConnectors();

    for(Index i = 0; i < buf_size; i++) assert(pages[i] == nullptr);
    start();
}

DocHandler::~DocHandler() {
    //delete scrolling_connector;
    //delete scene_connector;
}


unsigned int DocHandler::getCurrentPage() const { return current; }
int DocHandler::getLocation() const { return location; }
DocView* DocHandler::getView() const { return ui; }
BaseDocument* DocHandler::getDoc() const { return document; }


void DocHandler::resize(int new_value) {
    scale_factor = (double)new_value / 100;

    vector<Index> indexes = getIndexes();

    erasePages();

    document->setDpi(
        scale_factor * ui->physicalDpiX(),
        scale_factor * ui->physicalDpiY()
    );

    ui->getScene()->setSceneRect(
        0, 0, document->size()->width(), document->size()->height()
    );

    fillBuffer(indexes);

    for(PagePtr page: pages)
        if(page != nullptr && page->getIndex() == current)
            ui->centerOn(page);
}

void DocHandler::eraseFront(Index index) {
    PagePtr deleted = pages[0];
    if(deleted != nullptr) {
        document->page(index - buf_size)->cancelDrawn();
        ui->getScene()->removeItem(deleted);
    }
    pages.pop_front();
    delete deleted;
}

void DocHandler::eraseBack(Index index) {
    PagePtr deleted = pages[pages.size() - 1];
    if(deleted != nullptr) {
        document->page(index + buf_size)->cancelDrawn();
        ui->getScene()->removeItem(deleted);
    }
    pages.pop_back();
    delete deleted;
}

void DocHandler::drawNext(unsigned int index) {
    /*
        Рисует следующую страницу
        1. Создать графический элемент страницы
        2. Установить его сдвиг относительно начала документа
        3. Если буффер содержащий страницы заполнен, то
           удалить страницу с представления, удалить графический елемент
    */
    PagePtr page = new PageView(document->page(index)->render(), index);

    page->setOffset(document->page(index)->offset());
    ui->getScene()->addItem(page);
    pages.push_back(page);

    assert(pages.size() == buf_size + 1);
    eraseFront(index);
    assert(pages.size() == buf_size);
}

void DocHandler::drawPrev(unsigned int index) {
    /*
        Рисует предыдущую страницу
        Этот метод аналогичен предыдущему. Добавляет страницу не в конец
        а в начало буффера и соотвественно удаляет с конца а не с начала
    */
    PagePtr page = new PageView(document->page(index)->render(), index);

    page->setOffset(document->page(index)->offset());
    ui->getScene()->addItem(page);
    pages.push_front(page);

    assert(pages.size() == buf_size + 1);
    eraseBack(index);
    assert(pages.size() == buf_size);
}


void DocHandler::drawFirst() {
    vector<Index> indexes;
    unsigned int size = document->amountPages();
    if(size > buf_size) {
        for(Index i = 0; i < buf_size; i++) indexes.push_back(i);
        fillBuffer(indexes);
     } else {
        for(Index i = 0; i < size; i++) indexes.push_back(i);
        fillBuffer(indexes);
    }
    current = 0;
}


void DocHandler::start() {
    document->setDpi(
        scale_factor * ui->physicalDpiX(),
        scale_factor * ui->physicalDpiY()
    );
    document->init();
    ui->initScene(document->size());

    drawFirst();
    ui->centerOn(0, 0);
}


void DocHandler::fillBuffer(const vector<Index>& indexes) {
    for(Index i: indexes)
        drawNext(i);
}


vector<unsigned int> DocHandler::getIndexes() {
    vector<Index> res;
    for(PagePtr page: pages)
        if(page != nullptr)
            res.push_back(page->getIndex());
    return res;
}


void DocHandler::erasePages() {
    /* Полностью стирает страницы */
    for(PagePtr page: pages) {
        if(page != nullptr)
            ui->getScene()->removeItem(page);
        delete page;
    }
    pages.clear();
    Index len = document->amountPages();
    for(Index i = 0; i < len; i++)
        document->page(i)->cancelDrawn();

    assert(pages.size() == 0);
    for(Index i = 0; i < len; i++)
        assert(document->page(i)->isDrawn() == false);

    pages.resize(buf_size);
}


void DocHandler::goTo(unsigned int index) {
    current = index;
    emit pageChange(current);

    if(document->amountPages() < buf_size) {
        ui->centerOn(pages[current]);
        return;
    }

    erasePages();

    Index last = document->amountPages() - 1;
    Index len = document->amountPages();

    if(index + buf_size < last)
        for(Index i = index; i < index + buf_size; i++)
            drawNext(i);
    else
        for(Index i = last - buf_size; i < len; i++)
            drawNext(i);

    for(PagePtr page: pages)
       if(page->getIndex() == current)
           ui->centerOn(page);
}

void DocHandler::handleNext(int location) {
    if(location >= document->page(current)->bottom()) {
        if(current != document->amountPages() - 1) {
            current++;
            emit pageChange(current);
        }
        if(!document->page(current)->isDrawn())
            drawNext(current);
    }
}

void DocHandler::handlePrev(int location) {
    if(location <= document->page(current)->topY()) {
        if(current > 0) {
            current--;
            emit pageChange(current);
        }
        if(!document->page(current)->isDrawn())
            drawPrev(current);
    }
}


void DocHandler::initConnectors() {
    scrolling_connector = new One2One<DocView, DocHandler>(ui, this);
    scrolling_connector->connect(
        ui->getScrollSignals(), getScrollHandlers()
    );

    scroll_bar_connector = new One2One<ScrollBar, DocHandler>(
        ui->getScroll(),
        this
    );
    scroll_bar_connector->connect(
        ui->getScrollBarSignals(), getScrollBarHandler()
    );
}

const vector<void (DocHandler::*)(int)> DocHandler::getScrollHandlers() const {
    return {
        &DocHandler::onScrollUp,
        &DocHandler::onScrollDown
    };
}

const vector<void (DocHandler::*)(int)> DocHandler::getScrollBarHandler() const {
    return { &DocHandler::onScrollTriggered };
}



void DocHandler::onScrollTriggered(int action) {
    location = ui->getScroll()->value();
    switch(action) {
        case QAbstractSlider::SliderSingleStepAdd:
            handleNext(location + ui->height());
            return;
        case QAbstractSlider::SliderSingleStepSub:
            handlePrev(location);
            return;
        case QAbstractSlider::SliderPageStepAdd:
            handleNext(location + ui->height());
            return;
        case QAbstractSlider::SliderPageStepSub:
            handlePrev(location);
            return;
        case QAbstractSlider::SliderToMinimum:
            return;
        case QAbstractSlider::SliderToMaximum:
            return;
        case QAbstractSlider::SliderMove: {
            ui->getScroll()->setValue(location);
        } default: /*QAbstractSlider::SliderNoAction = 0*/ return;
    }
}



void DocHandler::onScrollDown(int step) {
    /*
            Обрабатывает скролинг вниз
        Если нижний край виджета ниже чем край страницы и
        она еще не отрисована, то нарисовать следующую страницу
    */
    location = ui->getScroll()->value();
    ui->getScroll()->setValue(location + step);
    handleNext(location + ui->height());
}


void DocHandler::onScrollUp(int step) {
    /*
            Обрабатывает скролинг вверх
        Если верх виджета выше верхнего края страницы и
        предыдущая страница не отрисована, то нарисовать ее
    */
    location = ui->getScroll()->value();
    ui->getScroll()->setValue(location - step);
    handlePrev(location);
}


const vector<void (DocHandler::*)(unsigned int)>
DocHandler::getPageChSignal() const {
    return {
        &DocHandler::pageChange,
    };
}
