#include <cassert>


#include "handlers.h"


MinimapHandler::MinimapHandler(const DocPtr &doc) : QObject() {
    document = doc;
    ui = new DocView;
    current = 0;

    document.get()->setDpi(20.0, 20.0);
    document.get()->init();
    pages.resize(BUF_SIZE);

    scene = new DocScene(document.get()->documentRect(), ui);
    scene->setBackgroundBrush(Qt::darkGray);
    ui->setScene(scene);
    start();

    initConnectors();

    assert(current == 0);
    assert(location == 0);
}

MinimapHandler::~MinimapHandler() {}


DocView* MinimapHandler::getMinimap() const { return ui; }


void MinimapHandler::start() {
    Size len = document.get()->amountPages();

    assert(pages.size() == BUF_SIZE);
    for(PageView* page: pages) assert(page == nullptr);

    if(len < BUF_SIZE) {
        for(Index i = 0; i < len; ++i)
            drawNext(i);
    } else {
        for(Index i = 0; i < BUF_SIZE; ++i)
            drawNext(i);

        for(PageView* page: pages)
            assert(page != nullptr);
    }
    ui->centerOn(0, 0);
}


void MinimapHandler::drawNext(Index index) {
    BaseDocument* _document = document.get();
    PDFPage* _page = _document->page(index);

    PageView* page = new PageView(_page->render(), index);
    page->setOffset(_page->getOffset());
    scene->addItem(page);
    pages.push_back(page);

    assert(pages.size() == BUF_SIZE + 1);

            /*Erase Front*/
    PageView* to_delete = pages[0];
    if(to_delete != nullptr) {
        _document->page(index - BUF_SIZE)->cancelDrawn();
        scene->removeItem(to_delete);
    }
    pages.pop_front();
    delete to_delete;

    assert(pages.size() == BUF_SIZE);
}


void MinimapHandler::drawPrev(Index index) {
    BaseDocument* _document = document.get();
    PDFPage* _page = _document->page(index);

    PageView* page = new PageView(_page->render(), index);
    page->setOffset(_page->getOffset());
    scene->addItem(page);
    pages.push_front(page);

    assert(pages.size() == BUF_SIZE + 1);

            /*Erase Back*/
    PageView* to_delete = pages[BUF_SIZE - 1];
    if(to_delete != nullptr) {
        _document->page(index + BUF_SIZE)->cancelDrawn();
        scene->removeItem(to_delete);
    }
    pages.pop_back();
    delete to_delete;

    assert(pages.size() == BUF_SIZE);
}


void MinimapHandler::initConnectors() {
    view2this.set(ui, this);
    view2this.connect<int>(
        vector<void (DocView::*)(int)>{
            &DocView::scrollDown, &DocView::scrollUp
        },
        vector<void (MinimapHandler::*)(int)>{
            &MinimapHandler::onScrollDown, &MinimapHandler::onScrollUp
        }
    );
}


void MinimapHandler::onScrollDown(int step) {
    BaseDocument* _document = document.get();
    PDFPage* _page = _document->page(current);

    location = ui->getScroll()->value();
    ui->getScroll()->setValue(location + step);

            /*Handler Next*/
    if(location + ui->height() >= _page->bottom()) {
        if(current != _document->amountPages() - 1)
            ++current;
        if(! _document->page(current)->isDrawn())
            drawNext(current);
    }
}


void MinimapHandler::onScrollUp(int step) {
    BaseDocument* _document = document.get();

    location = ui->getScroll()->value();
    ui->getScroll()->setValue(location - step);

            /*Handle Prev*/
    if(location <= _document->page(current)->topY()) {
        if(current > 0)
            --current;
        if(! _document->page(current)->isDrawn())
            drawPrev(current);
    }
}


void MinimapHandler::onScrollTriggered(int) {}
