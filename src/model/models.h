#ifndef MODELS_H
#define MODELS_H

#include <vector>
using std::vector;
#include <memory>
using std::shared_ptr;
using std::unique_ptr;
using std::make_shared;
#include <map>
using std::pair;

#include <QtCore/QObject>
#include <QtGui/QPixmap>
#include <QtCore/QSize>
#include <QtCore/QPoint>
#include <QtCore/QModelIndex>

#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>
#include <QtXml/QDomElement>

#include <QtGui/QStandardItem>
#include <QtGui/QStandardItemModel>

#include <poppler-qt5.h>

#include "../exceptions/exceptions.h"


static const double DEFAULT_DPI = 72.0;


using Index = unsigned int;
using Size = unsigned int;
using Indexes = vector<Index>;


class PDFPage {

    using Page = Poppler::Page;
    using Text = Poppler::TextBox;
    using TextPtr = shared_ptr<Text>;

    bool drawn;
    double dpi;
    double scale_factor;
    QPoint offset;
    QRect actual_rect;
    QRect base_rect;
    Page* page;

public:
    PDFPage(Page* _page, const QPoint& _offset, double _dpi);
    ~PDFPage(void);

    QSize size(void) const;
    QSize actualSize(void) const;
    QPoint getOffset(void) const;

    int topX(void) const;
    int topY(void) const;
    int actualHeight(void) const;
    int actualWidth(void) const;
    int baseHeight(void) const;
    int baseWidth(void) const;
    int bottom(void) const;
    int centerX(void) const;
    int centerY(void) const;

    QList<TextPtr> textBoxes() const;
    QList<pair<QRectF, QString>> actualTextBoxes() const;

    QList<QRectF> baseBoundingBoxes() const;
    QList<QRectF> actualBoundingBoxes() const;

    pair<QRectF, QString> getTextBox(const QPointF& point);

    pair<QRectF, QString> findExactOne(const QString& text);
    vector<pair<QRectF, QString>> findAll(const QString& text);

    bool isDrawn() const;
    void cancelDrawn();

    QPixmap render();
};



using Link = Poppler::LinkDestination;
class TocItem : public QStandardItem {
    Link link;

public:
    TocItem(const QString& name, const Link& _link);
    TocItem(const QString& name, Link* _link);
    ~TocItem() override;

    void set(const Link& _link);
    void set(Link* _link);

    int getPage(void) const { return link.pageNumber(); }
};


using PDFDocPtr = shared_ptr<Poppler::Document>;
class TocModel : public QStandardItemModel {
    Q_OBJECT

    PDFDocPtr document;

public:
    TocModel();
    TocModel(const PDFDocPtr& doc, const QString& name);
    ~TocModel() override;

    void set(const PDFDocPtr& doc, const QString& name);

private:
    void init(QDomDocument* toc);
    void build(QDomNode root, QStandardItem* model);
};



class BaseDocument : public QObject {
    Q_OBJECT

public:
    ~BaseDocument() = default;
    BaseDocument(const QString& p, const QString& n);

    QString getName(void) const;
    QString getPath(void) const;

    virtual void init(void) = 0;
    virtual void setDpi(double dpix, double dpiy) = 0;

    virtual QString metaInfo() const = 0;
    virtual QSize size(void) const = 0;
    virtual unsigned int amountPages(void) const = 0;
    virtual PDFPage* page(unsigned int index) const = 0;

    virtual bool operator ==(BaseDocument* other);
    virtual bool operator !=(BaseDocument* other);

protected:
    QString path;
    QString name;
};



class PDFDocument : public BaseDocument {
    Q_OBJECT

    PDFDocPtr document;
    int current;
    vector<unique_ptr<PDFPage>> pages;
    double dpix;
    double dpiy;
    int margin;
    QSize doc_size;

public:
    PDFDocument(const QString& path, const QString& name) noexcept(false);

    ~PDFDocument();

    void init(void) override;
    void setDpi(double dpix, double dpiy) override;

    QString metaInfo(void) const override;
    QSize size() const override;
    unsigned int amountPages() const override;
    PDFPage* page(unsigned int index) const override;
    PDFDocPtr getDocument(void) const { return document; }

private:
    void build(void);
    void rebuild(void);
};
#endif // MODELS_H
