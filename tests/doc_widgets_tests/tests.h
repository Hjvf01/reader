#ifndef TESTS_H
#define TESTS_H


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
    const QString base = "/home/roma/ws/C/scrs/e_reader/tests/samples";
    QUrl* path;
    DocWidget* widget;
    BaseDocument* doc;
    DocHandler* doc_ctrl;
    DocWidgetHandler* widget_ctrl;

public:
    virtual ~BaseTest() {
        delete path;
        delete widget;
        delete doc;
        delete doc_ctrl;
    }
};


class TestSinglePageDocWidget : public BaseTest {
    Q_OBJECT

public:
    TestSinglePageDocWidget();
    ~TestSinglePageDocWidget() override;
};


class TestMultPagesDocWidget : public BaseTest {
    Q_OBJECT

public:
    TestMultPagesDocWidget();
    ~TestMultPagesDocWidget() override;

private slots:
    void testResize(void);
};

#endif // TESTS_H
