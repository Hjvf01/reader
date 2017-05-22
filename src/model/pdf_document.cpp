#include "models.h"


PDFDocument::PDFDocument(QString p, QString n) : BaseDocument(p, n) {
    t_current = 0;
    t_dpix = 0.0;
    t_dpiy = 0.0;
    t_margin = 6;
    t_doc_size = new QSize(0, 0);
    t_toc = nullptr;
}

PDFDocument::~PDFDocument() {
    delete t_document;
    delete t_toc;
    delete t_doc_size;
    for(PDFPage* page: t_pages) delete page;
    for(auto p: destinations) delete p->second;
}

void PDFDocument::init(void) {
    if(t_dpix == 0 && t_dpiy == 0) throw "DPIDidntInit";

    t_document = Poppler::Document::load(path());
    if(t_document == nullptr) throw "DocumentDidntOpen";
    for(RenderHint hint: render_hints) t_document->setRenderHint(hint);
    build();
    t_toc = t_document->toc();
}

void PDFDocument::setDpi(double dpix, double dpiy) {
    /* Устанавливает значения DPI при инициализации или масштабировании */
    if(t_dpix == 0.0 && t_dpiy == 0.0) {
        t_dpix = dpix;
        t_dpiy = dpiy;
    } else {
        t_dpix = dpix;
        t_dpiy = dpiy;
        rebuild();
   }
}

void PDFDocument::rebuild() {
    /* Пересчитывает размеры страниц при масштабировании */
    for(auto page: t_pages) delete page;
    t_pages.clear();
    build();
}

void PDFDocument::build() {
    /*
        Строит список страниц документа
    */
    int len = t_document->numPages();
    int offset_x = 0;
    int offset_y = 0;
    for(int i = 0; i < len; i++) {
        t_pages.push_back(new PDFPage(
            t_document->page(i), offset_x, offset_y, t_dpix, t_dpiy
        ));
        int actual_height = t_pages[i]->actualHeight();
        int actual_width = t_pages[i]->actualWidth();
        t_doc_size->setHeight(
            (t_doc_size->height() + actual_height) + t_margin);
        if(actual_width > t_doc_size->width())
            t_doc_size->setWidth(actual_width);
        offset_x = 0; offset_y = t_doc_size->height();
    }
}

int PDFDocument::length() const { return t_doc_size->height(); }
int PDFDocument::width() const { return t_doc_size->width(); }
QSize* PDFDocument::size() const { return t_doc_size; }
vector<PDFPage*> PDFDocument::pages() const { return t_pages; }
double PDFDocument::scaleFactorX() { return t_dpix / DEFAULT_DPI; }
double PDFDocument::scaleFactorY() { return t_dpiy / DEFAULT_DPI; }

PDFPage* PDFDocument::page(unsigned int index) const {
    if(index > t_pages.size()) throw "IndexOutOfRange";
    return t_pages[index];
}

unsigned int PDFDocument::amountPages() const {
    return t_document->numPages();
}

QString PDFDocument::metaInfo() const {
    /* Вернет информацию о документе */
    QString out = "";
    QStringList keys = t_document->infoKeys();
    for(QString key: keys) out += key + ":\t" + t_document->info(key) + "\n";
    return out;
}

QStandardItemModel* PDFDocument::getToc() {
    QStandardItemModel* model = new QStandardItemModel;
    if(t_toc != nullptr) {
        QStandardItem* model_root = model->invisibleRootItem();
        QDomNode root = t_toc->firstChild();
        make_model(root, model_root);
    }
    return model;
}


void PDFDocument::make_model(QDomNode root, QStandardItem* model) {
    #define NOT !

    while(NOT root.nextSibling().isNull()) {
        QDomElement elem = root.toElement();
        QStandardItem* new_item = new QStandardItem(elem.tagName());
        model->appendRow(new_item);
        new_item->setEditable(false);

        if(NOT elem.attributeNode("Destination").isNull()) {
            destinations.push_back(new TOCPair(
                new_item,
                new TOCLink(
                    elem.attributeNode("Destination").value()
                )
            ));
        } else {
            destinations.push_back(new TOCPair(
                new_item,
                t_document->linkDestination(
                    elem.attributeNode("DestinationName").value()
                )
            ));
        }

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
