#include "models.h"


PDFPage::PDFPage(Page *_page, const QPoint& _offset, double _dpi)
        : page(_page), offset(_offset), dpi(_dpi), drawn(false) {

    scale_factor = dpi / DEFAULT_DPI;
    base_rect = QRect(offset, page->pageSize());
    actual_rect = QRect(
        offset.x(), offset.y(),
        scale_factor * page->pageSize().width(),
        scale_factor * page->pageSize().height()
    );
}

PDFPage::~PDFPage() {
    delete page;
}


QSize PDFPage::size() const {
    return QSize(base_rect.width(), base_rect.height());
}

QSize PDFPage::actualSize() const {
    return QSize(actual_rect.width(), actual_rect.height());
}

QPoint PDFPage::getOffset() const { return offset; }


int PDFPage::topX() const { return offset.x(); }
int PDFPage::topY() const { return offset.y(); }

int PDFPage::actualHeight() const { return actual_rect.height(); }
int PDFPage::actualWidth() const { return actual_rect.width(); }
int PDFPage::baseHeight() const { return base_rect.height(); }
int PDFPage::baseWidth() const { return base_rect.width(); }

int PDFPage::bottom() const { return offset.y() + actual_rect.height(); }

int PDFPage::centerY() const { return offset.y() + actual_rect.height() / 2; }
int PDFPage::centerX() const { return actual_rect.height() / 2; }

bool PDFPage::isDrawn() const { return drawn; }
void PDFPage::cancelDrawn() { drawn = false; }


QList<QRectF> PDFPage::baseBoundingBoxes() const {
    /* Вернет список прямоугольников содержащих текст
     * (при значении DPI=72.0) */
    QList<QRectF> list;
    QList<Text*> boxes = page->textList();
    for(Text* box: boxes)
        list.append(QRectF(
            offset.x() + box->boundingBox().x(),
            offset.y() + box->boundingBox().y(),
            box->boundingBox().width(),
            box->boundingBox().height()
        ));
    for(Text* box: boxes) delete box;
    return list;
}

QList<QRectF> PDFPage::actualBoundingBoxes() const {
    /* Вернет список прямоугольников содержащих текст
     * (при актуальном значении DPI) */
    QList<QRectF> list;
    QList<Text*> boxes = page->textList();
    for(Text* box: boxes)
        list.append(QRectF(
            offset.x() + scale_factor * box->boundingBox().x(),
            offset.y() + scale_factor * box->boundingBox().y(),
            scale_factor * box->boundingBox().width(),
            scale_factor * box->boundingBox().height()
        ));
    for(Text* box: boxes) delete box;
    return list;
}


pair<QRectF, QString> PDFPage::getTextBox(const QPointF& point) {
    /*Вернет пару(прямоугольник, текст)*/
    for(pair<QRectF, QString> box: actualTextBoxes())
        if(box.first.contains(point)) return box;
    return pair<QRectF, QString>(
        QRectF(0.0, 0.0, 0.0, 0.0), QString()
    );
}

QList<shared_ptr<Poppler::TextBox>> PDFPage::textBoxes() const {
    QList<TextPtr> out;
    for(Text* text_box: page->textList())
        out.append(TextPtr(text_box));
    return out;
}

QList<pair<QRectF, QString>> PDFPage::actualTextBoxes() const {
    /* Вернет пару(пряммоугольник, текст)
     * при актуальном значении dpi */
    QList<pair<QRectF, QString>> list;
    QList<Text*> boxes = page->textList();
    for(Text* box: boxes)
        list.append(pair<QRectF, QString>(
            QRectF(
                 offset.x() + scale_factor * box->boundingBox().x(),
                 offset.y() + scale_factor * box->boundingBox().y(),
                 scale_factor * box->boundingBox().width(),
                 scale_factor * box->boundingBox().height()
            ),
            QString(box->text())
        ));

    for(Text* box: boxes) delete box;
    return list;
}


pair<QRectF, QString> PDFPage::findExactOne(const QString& text) {
    for(auto box: actualTextBoxes())
        if(box.second == text) return box;

    return pair<QRectF, QString>(QRectF(), QString());
}

vector<pair<QRectF, QString>> PDFPage::findAll(const QString& text) {
    vector<pair<QRectF, QString>> result;
    for(auto box: actualTextBoxes())
        if(box.second == text)
            result.push_back(box);

    return result;
}


QPixmap PDFPage::render() {
    /* вернет изображение страницы */
    drawn = true;
    return QPixmap::fromImage(page->renderToImage(dpi, dpi));
}
