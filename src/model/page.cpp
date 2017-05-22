#include "models.h"


PDFPage::PDFPage(Page *page, int o_x, int o_y, double dpix, double dpiy) {
    this->page = page;
    width = this->page->pageSize().width();
    height = this->page->pageSize().height();
    actual_height = (height * dpiy) / DEFAULT_DPI;
    actual_width = (width * dpix) / DEFAULT_DPI;
    offset_x = o_x;
    offset_y = o_y;
    this->dpix = dpix;
    this->dpiy = dpiy;
    t_offset = new QPoint(o_x, o_y);
    drawn = false;
    scale_factor_x = dpix / DEFAULT_DPI;
    scale_factor_y = dpiy / DEFAULT_DPI;
}

PDFPage::~PDFPage() { delete page; delete t_offset; }


QSize PDFPage::size() const { return QSize(width, height); }
QSize PDFPage::actualSize() const { return QSize(actual_width, actual_height); }
QPoint PDFPage::offset() const { return *t_offset; }


int PDFPage::topX() const { return offset_x; }
int PDFPage::topY() const { return offset_y; }

int PDFPage::actualHeight() const { return actual_height; }
int PDFPage::actualWidth() const { return actual_width; }
int PDFPage::baseHeight() const { return height; }
int PDFPage::baseWidth() const { return width; }

int PDFPage::bottom() const { return offset_y + actual_height; }

int PDFPage::centerY() const { return offset_y + actual_height / 2; }
int PDFPage::centerX() const { return actual_height / 2; }

bool PDFPage::isDrawn() const { return drawn; }
void PDFPage::cancelDrawn() { drawn = false; }


QList<QRectF> PDFPage::baseBoundingBoxes() const {
    /* Вернет список прямоугольников содержащих текст
     * (при значении DPI=72.0) */
    QList<QRectF> list;
    QList<Text*> boxes = page->textList();
    for(Text* box: boxes)
        list.append(QRectF(
            offset_x + box->boundingBox().x(),
            offset_y + box->boundingBox().y(),
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
            offset_x + scale_factor_x * box->boundingBox().x(),
            offset_y + scale_factor_y * box->boundingBox().y(),
            scale_factor_x * box->boundingBox().width(),
            scale_factor_y * box->boundingBox().height()
        ));
    for(Text* box: boxes) delete box;
    return list;
}


pair<QRectF, QString> PDFPage::getTextBox(QPointF point) {
    /*Вернет пару(прямоугольник, текст)*/
    for(pair<QRectF, QString> box: actualTextBoxes())
        if(box.first.contains(point)) return box;
    return pair<QRectF, QString>(
        QRectF(0.0, 0.0, 0.0, 0.0),
        QString()
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
                 offset_x + scale_factor_x * box->boundingBox().x(),
                 offset_y + scale_factor_y * box->boundingBox().y(),
                 scale_factor_x * box->boundingBox().width(),
                 scale_factor_y * box->boundingBox().height()
            ),
            QString(box->text())
        ));

    for(Text* box: boxes) delete box;
    return list;
}


QPixmap PDFPage::render() {
    /* вернет изображение страницы */
    drawn = true;
    return QPixmap::fromImage(page->renderToImage(dpix, dpiy));
}
