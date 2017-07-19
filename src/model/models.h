#ifndef MODELS_H
#define MODELS_H

#include <vector>
using std::vector;
#include <memory>
using std::shared_ptr;
using std::unique_ptr;
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


static const double DEFAULT_DPI = 72.0;


using Index = unsigned int;
using Size = unsigned int;
using Indexes = vector<Index>;


class PDFPage {

    using Page = Poppler::Page;
    using Text = Poppler::TextBox;
    using TextPtr = shared_ptr<Text>;

    bool drawn;
    int width;
    int height;
    int actual_width;
    int actual_height;
    int offset_x;
    int offset_y;
    double dpix;
    double dpiy;
    double scale_factor_x;
    double scale_factor_y;
    QPoint* t_offset;
    Page* page;

public:
    PDFPage(Page* page, int o_x, int o_y, double dpix, double dpiy);
    ~PDFPage(void);

    QSize size(void) const;
    QSize actualSize(void) const;
    QPoint offset(void) const;

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


class BaseDocument : public QObject {
    Q_OBJECT

public:
    ~BaseDocument() = default;
    BaseDocument(QString p, QString n);

    QString getName(void) const;
    QString getPath(void) const;

    virtual void init(void) = 0;
    virtual void setDpi(double dpix, double dpiy) = 0;

    virtual int length(void) const = 0;
    virtual QString metaInfo() const = 0;
    virtual int width(void) const = 0;
    virtual QSize* size(void) const = 0;
    virtual unsigned int amountPages(void) const = 0;
    virtual vector<PDFPage*> getPages() const = 0;
    virtual PDFPage* page(unsigned int index) const = 0;
    virtual double scaleFactorX() = 0;
    virtual double scaleFactorY() = 0;

    virtual QStandardItemModel* getToc(void) = 0;
    virtual int getPage(const QModelIndex& index) const = 0;

    virtual bool operator ==(BaseDocument* other);
    virtual bool operator !=(BaseDocument* other);

protected:
    QString path;
    QString name;
};


class TocItem : public QStandardItem {
    Q_OBJECT

public:
    TocItem(const Poppler::LinkDestination& link);
    ~TocItem() override;
};


class TocModel : public QStandardItemModel {
    Q_OBJECT

public:
    TocModel(QDomDocument* dom, const QString& name);
    ~TocModel() override;

private:
    void build(QDomNode root, QStandardItem* model);
};


class PDFDocument : public BaseDocument {
    Q_OBJECT

    using RenderHint = Poppler::Document::RenderHint;
    using RenderHints = vector<RenderHint>;
    using TOCLink = Poppler::LinkDestination;
    using TOCPair = pair<QStandardItem*, TOCLink*>;

    Poppler::Document* document;
    int current;
    vector<PDFPage*> pages;
    double dpix;
    double dpiy;
    int margin;
    QSize* doc_size;
    QStandardItemModel* toc_model;

    const RenderHints render_hints = {
        RenderHint::Antialiasing,
        RenderHint::TextAntialiasing,
        RenderHint::TextHinting,
        RenderHint::TextSlightHinting
    };
    vector<TOCPair*> destinations;

public:
    PDFDocument(QString p, QString n);
    ~PDFDocument();

    void init(void) override;
    void setDpi(double dpix, double dpiy) override;

    int length(void) const override;
    QString metaInfo(void) const override;
    int width() const override;
    QSize* size() const override;
    unsigned int amountPages() const override;
    vector<PDFPage*> getPages() const override;
    PDFPage* page(unsigned int index) const override;

    QStandardItemModel* getToc() override;
    int getPage(const QModelIndex& index) const override;

    double scaleFactorX() override;
    double scaleFactorY() override;

private:
    void rebuild(void);
    void build(void);

    void make_model(QDomNode root, QStandardItem* model);
};
#endif // MODELS_H
