#include <cassert>

#include <QtWidgets/QAbstractSlider>
#include <Qt>

#include "handlers.h"


DocHandler::DocHandler(const DocPtr& doc) : QObject() {
    #define DOC_PTR document.get()
    /*
        1. Размер буфера страниц
        2. Представление этого обработчика
        3. текущая страница
        4. Дек, содержащий страницы
        5. текущее расположени верха виджета относительно начала сцены
        6. Масштаб
    */
    ui = new DocView;
    handler = new SceneHandler(doc);
    current = 0;
    document = doc;
    location = 0;
    scale_factor = 1.0;
    pages.resize(buf_size);

    initConnectors();

    for(Index i = 0; i < buf_size; i++)
        assert(pages[i] == nullptr);

    start();
}

DocHandler::~DocHandler() {
    for(PagePtr page: pages) {
        if(page != nullptr)
            handler->getScene()->removeItem(page);
        delete page;
    }
    pages.clear();

    delete ui;
    delete handler;
}


unsigned int DocHandler::getCurrentPage() const { return current; }
int DocHandler::getLocation() const { return location; }
DocView* DocHandler::getView() const { return ui; }
SceneHandler* DocHandler::getSceneHandler() const { return handler; }


void DocHandler::resize(int new_value) {
    scale_factor = (double)new_value / 100;

    vector<Index> indexes = getIndexes();

    erasePages();

    DOC_PTR->setDpi(
        scale_factor * ui->physicalDpiX(),
        scale_factor * ui->physicalDpiY()
    );

    handler->getScene()->setSceneRect(
        0, 0, DOC_PTR->size().width(), DOC_PTR->size().height()
    );

    fillBuffer(indexes);

    centerOnCurrentPage();
}

void DocHandler::eraseFront(Index index) {
    PagePtr deleted = pages[0];
    if(deleted != nullptr) {
        DOC_PTR->page(index - buf_size)->cancelDrawn();
        handler->getScene()->removeItem(deleted);
    }
    pages.pop_front();
    delete deleted;
}

void DocHandler::eraseBack(Index index) {
    PagePtr deleted = pages[pages.size() - 1];
    if(deleted != nullptr) {
        DOC_PTR->page(index + buf_size)->cancelDrawn();
        handler->getScene()->removeItem(deleted);
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
    PagePtr page = new PageView(DOC_PTR->page(index)->render(), index);

    page->setOffset(DOC_PTR->page(index)->getOffset());
    handler->getScene()->addItem(page);
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
    PagePtr page = new PageView(DOC_PTR->page(index)->render(), index);

    page->setOffset(DOC_PTR->page(index)->getOffset());
    handler->getScene()->addItem(page);
    pages.push_front(page);

    assert(pages.size() == buf_size + 1);
    eraseBack(index);
    assert(pages.size() == buf_size);
}


void DocHandler::drawFirst() {
    vector<Index> indexes;
    unsigned int size = DOC_PTR->amountPages();
    if(size > buf_size) {
        for(Index i = 0; i < buf_size; i++)
            indexes.push_back(i);
        fillBuffer(indexes);
     } else {
        for(Index i = 0; i < size; i++)
            indexes.push_back(i);
        fillBuffer(indexes);
    }
    current = 0;
}


void DocHandler::start() {
    document.get()->setDpi(
        scale_factor * ui->physicalDpiX(),
        scale_factor * ui->physicalDpiY()
    );
    document.get()->init();
    handler->getScene()->setSceneRect(document.get()->documentRect());
    ui->setScene(handler->getScene());

    drawFirst();
    ui->centerOn(0, 0);
}


void DocHandler::fillBuffer(const vector<Index>& indexes) {
    for(Index i: indexes)
        drawNext(i);
}


vector<unsigned int> DocHandler::getIndexes() const {
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
            handler->getScene()->removeItem(page);
        delete page;
    }
    pages.clear();
    Index len = DOC_PTR->amountPages();
    for(Index i = 0; i < len; i++)
        DOC_PTR->page(i)->cancelDrawn();

    assert(pages.size() == 0);
    for(Index i = 0; i < len; i++)
        assert(DOC_PTR->page(i)->isDrawn() == false);

    pages.resize(buf_size);
}


void DocHandler::goToNext() {
    if(current == document.get()->amountPages() - 1)
        return;

    if(document.get()->page(current + 1)->isDrawn())
        for(PagePtr page: pages)
            if(page != nullptr && page->getIndex() == current + 1) {
                ++current;
                emit pageChange(current);
                centerOnCurrentPage();
                return;
            }

    ++current;
    emit pageChange(current);
    drawNext(current);
    centerOnCurrentPage();
}

void DocHandler::goToPrev() {
    if(current == 0)
        return;

    if(document.get()->page(current - 1)->isDrawn())
        for(PagePtr page: pages)
            if(page != nullptr && page->getIndex() == current - 1) {
                --current;
                emit pageChange(current);
                centerOnCurrentPage();
                return;
            }

    --current;
    emit pageChange(current);
    drawPrev(current);
    centerOnCurrentPage();
}

void DocHandler::goTo(unsigned int index) {
    /*
        1. Если новый индекс меньше текущего на 1, goToPrev
        2. Если новый индекс больше текущего на 1, goToNext
        3. Иначе перерисовать страницы.
    */
    if(index == current - 1) return goToPrev();
    if(index == current + 1) return goToNext();

    current = index;
    if(document.get()->amountPages() < buf_size) {
        centerOnCurrentPage();
        return;
    }

    unsigned int len = document.get()->amountPages();
    unsigned int last = len - 1;

    erasePages();
    if(index + buf_size < last)
        for(unsigned int i = index; i < index + buf_size; ++i)
            drawNext(i);
    else
        for(unsigned int i = last - buf_size; i < len; i++)
            drawNext(i);
    centerOnCurrentPage();
    emit pageChange(current);
}


void DocHandler::centerOnCurrentPage() const {
    for(PagePtr page: pages)
        if(page != nullptr && page->getIndex() == current)
            ui->centerOn(
                document.get()->page(current)->centerX(),
                document.get()->page(current)->centerY()
            );
}


void DocHandler::handleNext(int location) {
    if(location >= DOC_PTR->page(current)->bottom()) {
        if(current != DOC_PTR->amountPages() - 1) {
            current++;
            emit pageChange(current);
        }
        if(! DOC_PTR->page(current)->isDrawn())
            drawNext(current);
    }
}

void DocHandler::handlePrev(int location) {
    if(location <= DOC_PTR->page(current)->topY()) {
        if(current > 0) {
            current--;
            emit pageChange(current);
        }
        if(! DOC_PTR->page(current)->isDrawn())
            drawPrev(current);
    }
}


void DocHandler::initConnectors() {
    scrolling_connector.set(ui, this);
    scrolling_connector.connect<int>(
        vector<void (DocView::*)(int)>{
            &DocView::scrollUp,
            &DocView::scrollDown
        },
        vector<void (DocHandler::*)(int)>{
            &DocHandler::onScrollUp,
            &DocHandler::onScrollDown
        }
    );

    scroll_bar_connector.set(ui->getScroll(), this);
    scroll_bar_connector.connect<int>(
        vector<void (ScrollBar::*)(int)>{&ScrollBar::actionTriggered},
        vector<void (DocHandler::*)(int)>{&DocHandler::onScrollTriggered}
    );
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
