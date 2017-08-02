#include <cassert>
#include <string>
using std::to_string;

#include <QtCore/QDebug>

#include "models.h"


PDFDocument::PDFDocument(const QString& path, const QString& name)
       noexcept(false) : BaseDocument(path, name) {
    current = 0;
    dpix = 0.0;
    dpiy = 0.0;
    margin = 6;

    document = PDFDocPtr(Poppler::Document::load(path));
    if(document.get() == nullptr)
        throw DocumentWasntOpened(
            "document '" + name.toStdString() +
            "' wasn't opened."
        );

    pages.resize(document.get()->numPages());
}

PDFDocument::~PDFDocument() {}

void PDFDocument::init(void) {
    if(dpix == 0 && dpiy == 0)
        throw DPIWasntInit(
            "Dpi wansn't init in '" + name.toStdString() +
            "' document"
        );

    using RenderHint = Poppler::Document::RenderHint;
    const vector<RenderHint> render_hints = {
        RenderHint::Antialiasing, RenderHint::TextAntialiasing,
        RenderHint::TextHinting, RenderHint::TextSlightHinting
    };

    for(RenderHint hint: render_hints)
        document->setRenderHint(hint);
    build();
}


void PDFDocument::setDpi(double dx, double dy) {
    /* Устанавливает значения DPI при инициализации или масштабировании */
    if(dpix == 0.0 && dpiy == 0.0) {
        /* инициализация */
        dpix = dx;
        dpiy = dy;
    } else {
        /* масштабирование */
        dpix = dx;
        dpiy = dy;
        rebuild();
   }
}


void PDFDocument::rebuild() {
    /* Пересчитывает размеры страниц при масштабировании */
    pages.clear();
    assert(pages.size() == 0);

    pages.resize(document.get()->numPages());
    build();
}


void PDFDocument::build() {
    /* Строит список страниц документа */

    doc_size.setWidth(0);
    doc_size.setHeight(0);

    unsigned int len = document->numPages();
    unsigned int offset_y = 0;
    assert(pages.size() == len);
    for(unsigned int i = 0; i < len; i++) {
        pages[i] = unique_ptr<PDFPage>(new PDFPage(
            document.get()->page(i), QPoint(0, offset_y), dpix
        ));
        int actual_height = pages[i].get()->actualHeight();
        int actual_width = pages[i].get()->actualWidth();
        doc_size.setHeight(
            (doc_size.height() + actual_height) + margin);
        if(actual_width > doc_size.width())
            doc_size.setWidth(actual_width);
        offset_y = doc_size.height();
    }
}


QSize PDFDocument::size() const { return doc_size; }


QRectF PDFDocument::documentRect() const {
    return QRectF(0, 0, doc_size.width(), doc_size.height());
}


PDFPage* PDFDocument::page(unsigned int index) const {
    if(index > pages.size())
        throw PageOutOfRange(
            to_string(index) + " out of amount pages(" +
            to_string(document.get()->numPages()) + string(").")
        );

    return pages[index].get();
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
