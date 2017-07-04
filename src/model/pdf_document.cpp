#include "models.h"


PDFDocument::PDFDocument(QString p, QString n) : BaseDocument(p, n) {
    current = 0;
    dpix = 0.0;
    dpiy = 0.0;
    margin = 6;
    doc_size = new QSize(0, 0);
    toc_model = new QStandardItemModel;
}

PDFDocument::~PDFDocument() {
    delete document;
    delete doc_size;
    delete toc_model;

    for(PDFPage* page: pages) delete page;
    for(auto p: destinations) delete p->second;
    for(auto p: destinations) delete p;
}

void PDFDocument::init(void) {
    if(dpix == 0 && dpiy == 0)
        throw "DPIDidntInit";

    document = Poppler::Document::load(path);
    if(document == nullptr)
        throw "DocumentDidntOpen";

    for(RenderHint hint: render_hints)
        document->setRenderHint(hint);
    build();
}

void PDFDocument::setDpi(double dx, double dy) {
    /* Устанавливает значения DPI при инициализации или масштабировании */
    if(dpix == 0.0 && dpiy == 0.0) {
        dpix = dx;
        dpiy = dy;
    } else {
        dpix = dx;
        dpiy = dy;
        doc_size->setHeight(0);
        doc_size->setWidth(0);
        rebuild();
   }
}

void PDFDocument::rebuild() {
    /* Пересчитывает размеры страниц при масштабировании */
    for(auto page: pages)
        delete page;

    pages.clear();
    build();
}

void PDFDocument::build() {
    /* Строит список страниц документа */
    int len = document->numPages();
    int offset_x = 0;
    int offset_y = 0;
    for(int i = 0; i < len; i++) {
        pages.push_back(new PDFPage(
            document->page(i), offset_x, offset_y, dpix, dpiy
        ));
        int actual_height = pages[i]->actualHeight();
        int actual_width = pages[i]->actualWidth();
        doc_size->setHeight(
            (doc_size->height() + actual_height) + margin);
        if(actual_width > doc_size->width())
            doc_size->setWidth(actual_width);
        offset_x = 0; offset_y = doc_size->height();
    }
}

int PDFDocument::length() const { return doc_size->height(); }
int PDFDocument::width() const { return doc_size->width(); }
QSize* PDFDocument::size() const { return doc_size; }
vector<PDFPage*> PDFDocument::getPages() const { return pages; }
double PDFDocument::scaleFactorX() { return dpix / DEFAULT_DPI; }
double PDFDocument::scaleFactorY() { return dpiy / DEFAULT_DPI; }

PDFPage* PDFDocument::page(unsigned int index) const {
    if(index > pages.size()) throw "IndexOutOfRange";
    return pages[index];
}

unsigned int PDFDocument::amountPages() const {
    return document->numPages();
}

QString PDFDocument::metaInfo() const {
    /* Вернет информацию о документе */
    QString out = "";
    QStringList keys = document->infoKeys();
    for(QString key: keys)
        out += key + ":\t" + document->info(key) + "\n";

    return out;
}

QStandardItemModel* PDFDocument::getToc() {
    toc_model->setHorizontalHeaderLabels(QStringList(name));
    QDomDocument* doc_toc = document->toc();
    if(doc_toc != nullptr) {
        QStandardItem* model_root = toc_model->invisibleRootItem();
        QDomNode root = doc_toc->firstChild();
        make_model(root, model_root);
    }
    return toc_model;
}


void PDFDocument::make_model(QDomNode root, QStandardItem* model) {
    #define NOT !

    while(NOT root.nextSibling().isNull()) {
        QDomElement elem = root.toElement();
        QStandardItem* new_item = new QStandardItem(elem.tagName());
        model->appendRow(new_item);
        new_item->setEditable(false);

        if(NOT elem.attributeNode("Destination").isNull())
            destinations.push_back(new TOCPair(
                new_item,
                new TOCLink(
                    elem.attributeNode("Destination").value()
                )
            ));
        else
            destinations.push_back(new TOCPair(
                new_item,
                document->linkDestination(
                    elem.attributeNode("DestinationName").value()
                )
            ));

        if(NOT root.firstChild().isNull()) {
            make_model(root.firstChild(), new_item);
        }
        root = root.nextSibling();
    }

    #undef NOT
}


int PDFDocument::getPage(const QModelIndex& index) const {
    for(auto link: destinations)
        if(link->first->index() == index)
            return link->second->pageNumber() - 1;

    return 0;
}
