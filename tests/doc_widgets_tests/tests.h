#ifndef TESTS_H
#define TESTS_H

#include <functional>
using std::function;
#include <cmath>
using std::fabs;


#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QUrl>

#include <QtTest/QTest>

#include "../../src/handlers/handlers.h"
#include "../../src/ui/ui.h"
#include "../../src/model/models.h"


class BaseTest : public QObject {
    Q_OBJECT

protected:
    using Index = unsigned int;

    const QString base = "/home/roma/ws/C/scrs/e_reader/tests_src/samples";
    QUrl* path;
    DocWidget* widget;
    BaseDocument* doc;
    DocHandler* doc_ctrl;
    DocWidgetHandler* widget_ctrl;

public:
    BaseTest(QString name) {
        path = new QUrl(base + name);
        widget = new DocWidget;
        doc = new PDFDocument(path->path(), path->fileName());
        doc_ctrl = new DocHandler(widget->getView(), doc);
        widget_ctrl = new DocWidgetHandler(widget, doc_ctrl);

        widget->show();
    }

    virtual ~BaseTest() {
        delete path;
        delete widget;
        delete doc;
        delete doc_ctrl;
        delete widget_ctrl;
    }
};


class TestSinglePageDocWidget : public BaseTest {
    Q_OBJECT

public:
    TestSinglePageDocWidget(QString name);
    ~TestSinglePageDocWidget() override;

private slots:
    void testResize(void);
};


class TestMultPagesDocWidget : public BaseTest {
    Q_OBJECT

public:
    TestMultPagesDocWidget(QString name);
    ~TestMultPagesDocWidget() override;

private slots:
    void testGoTo(void);
    void testOnLastPage(void);
    void testOnFirstPage(void);
    void testOnNextPage(void);
    void testOnPrevPage(void);

    void testResize(void);
private:
    void pagesAreDrawn(Index fst, Index scd, Index thrd, Index i);
};

#endif // TESTS_H
