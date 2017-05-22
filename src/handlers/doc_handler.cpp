#include <cassert>

#include "handlers.h"


void docViewConnector(DocView *sender, DocHandler *receiver) {
    /* Соединяет сигналы представления документа с его обработчиком */
    QObject::connect(
        sender, &DocView::scrollUp, receiver, &DocHandler::onScrollUp
    );
    QObject::connect(
        sender, &DocView::scrollDown, receiver, &DocHandler::onScrollDown
    );
    QObject::connect(
        sender->getScene(), &DocScene::doubleClick,
        receiver, &DocHandler::onDoubleClick
    );
}


DocHandler::DocHandler(DocView* ui, BaseDocument* doc) : QObject() {
    /*
        1. Размер буфера страниц
        2. Представление этого обработчика
        3. текущая страница
        4. Дек, содержащий страницы
        5. текущее расположени верха виджета относительно начала сцены
        6. Масштаб
    */
    buf_size = 3;
    this->ui = ui;
    current = 0;
    document = doc;
    pages = deque<PagePtr>(buf_size);
    location = 0;
    scale_factor = 1.0;

    for(Index i = 0; i < buf_size; i++) assert(pages[i] == nullptr);
    start();
}

DocHandler::~DocHandler() {/*Этот класс не владеет ни одним указателем*/}


int DocHandler::getCurrentPage() const { return current; }
int DocHandler::getLocation() const { return location; }
DocView* DocHandler::getView() const { return ui; }
BaseDocument* DocHandler::getDoc() const { return document; }


void DocHandler::resize(double new_value) {
    qDebug() << document->name() << " resizing: " << new_value;
}


bool DocHandler::pointBeyondScene(float x, float y) {
    #define OR ||

    return (
        (x > ui->getScene()->sceneRect().width()) OR
        (y > ui->getScene()->sceneRect().height()) OR
        (x < 0) OR
        (y < 0)
    );

    #undef OR
}


void DocHandler::onDoubleClick(QPointF point) {
    /*
        Обрабатывает двойной клик на сцене
        Если он пришелся на слово то автоматически переводит его
    */
    if(pointBeyondScene(point.x(), point.y()))
        return;
    for(Page* page_elem: pages)
        if(page_elem != nullptr && page_elem->page->contains(point)) {
            pair<QRectF, QString> elem =
                document->page(page_elem->index)->getTextBox(point);
            if(elem.first.width() == 0 && elem.first.height() == 0)
                return;
            ui->getScene()->setHightLight(elem.first);
            emit translate(elem.second);
            emit lookup(elem.second);
            tr_dialog.setWindowTitle(elem.second);

            return;
    }
}


void DocHandler::onScrollDown(int step) {
    /*
            Обрабатывает скролинг вниз
        Если нижний край виджета ниже чем край страницы и
        она еще не отрисована, то нарисовать следующую страницу
    */
    location = ui->getScroll()->value();
    int bottom = location + ui->height();
    if(bottom >= document->page(current)->bottom()) {
        if(current != document->amountPages() - 1) {
            current++;
            emit pageChange(current);
        }
        if(!document->page(current)->isDrawn())
            drawNext(current);
    }
    ui->getScroll()->setValue(location + step);
}


void DocHandler::onScrollUp(int step) {
    /*
            Обрабатывает скролинг вверх
        Если верх виджета выше верхнего края страницы и
        предыдущая страница не отрисована, то нарисовать ее
    */
    location = ui->getScroll()->value();
    if(location <= document->page(current)->topY()) {
        if(current > 0) {
            current--;
            emit pageChange(current);
        }
        if(!document->page(current)->isDrawn())
            drawPrev(current);
    }
    ui->getScroll()->setValue(location - step);
}


void DocHandler::eraseFront(Index index) {
    PagePtr deleted = pages[0];
    if(deleted != nullptr) {
        document->page(index - buf_size)->cancelDrawn();
        ui->getScene()->removeItem(deleted->page);
    }
    pages.pop_front();
    delete deleted;
}

void DocHandler::eraseBack(Index index) {
    PagePtr deleted = pages[pages.size() - 1];
    if(deleted != nullptr) {
        document->page(index + buf_size)->cancelDrawn();
        ui->getScene()->removeItem(deleted->page);
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
    PagePixPtr page_pix = new QGraphicsPixmapItem(
        document->page(index)->render()
    );
    PagePtr page = new Page(index, page_pix);

    page_pix->setOffset(document->page(index)->offset());
    ui->getScene()->addItem(page_pix);
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
    PagePixPtr page_pix = new QGraphicsPixmapItem(
        document->page(index)->render()
    );
    PagePtr page = new Page(index, page_pix);

    page_pix->setOffset(document->page(index)->offset());
    ui->getScene()->addItem(page_pix);
    pages.push_front(page);

    assert(pages.size() == buf_size + 1);
    eraseBack(index);
    assert(pages.size() == buf_size);
}


void DocHandler::drawFirst() {
    /*
        Отрисовавает первые страницы.
    */
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
    connect(
        &tr_dialog, &TrDialog::closeDialog, this, &DocHandler::onDialogClose
    );
}


void DocHandler::fillBuffer(vector<Index> indexes) {
    for(Index i: indexes) drawNext(i);
}


vector<unsigned int> DocHandler::getIndexes() {
    vector<Index> res;
    for(PagePtr page: pages)
        res.push_back(page->index);
    return res;
}


void DocHandler::goTo(unsigned int index) {
    for(Index i: getIndexes()) eraseBack(i);
    assert(pages.size() == 0);
    for(Index i = 0; i < buf_size; i++) pages.push_back(nullptr);
    for(auto page: pages) assert(page == nullptr);
    for(register Index i = 0; i < document->amountPages(); i++)
        document->page(i)->cancelDrawn();

    current = index;
    Index last = document->amountPages() - 1;
    if(document->amountPages() < buf_size) {
        drawFirst();
        ui->centerOn(pages[current]->page);
        return;
    } if(index < buf_size) {
        drawFirst();
        ui->centerOn(pages[current]->page);
        return;
    } if(index + buf_size < document->amountPages() - 1) {
        for(Index i = index; i < index + buf_size; i++)
            drawNext(i);
        for(auto page: pages)
            if(page->index == current) ui->centerOn(page->page);
        return;
    } if(index > document->amountPages() - buf_size) {
        for(Index i = last - buf_size; i < document->amountPages(); i++)
            drawNext(i);
        for(auto page: pages)
            if(page->index == current) ui->centerOn(page->page);
        return;
    }
}


void DocHandler::onError(QString error_msg) {
    qDebug() << error_msg;
}

void DocHandler::onTranslateReady(const QJsonObject result) {
    /* принимает результаты перевода и отбражает их в диалогов окне */
    tr_dialog.setTranslate(result);
    if(dialog_shown == false) tr_dialog.show();
    dialog_shown = true;
}

void DocHandler::onLookupReady(const QJsonObject result) {
    /* аналогично, но результаты поиска в словаре */
    tr_dialog.setLookup(result);
    if(dialog_shown == false) tr_dialog.show();
    dialog_shown = true;
}

void DocHandler::onDialogClose() {
    dialog_shown = false;
    ui->getScene()->eraseHightlight();
}
