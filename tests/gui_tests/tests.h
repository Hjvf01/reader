#pragma once

#include <iostream>
using std::cout;
using std::endl;
#include <vector>
using std::vector;
#include <memory>
using std::shared_ptr;

#include <QtTest/QtTest>

#include <QtCore/QObject>
#include <QtCore/QPointF>
#include <QtCore/QUrl>
#include <QtCore/QRectF>
#include <QtCore/Qt>

#include <QtWidgets/QApplication>

#include <QtWidgets/QGraphicsRectItem>

#include <QtGui/QBrush>
#include <QtGui/QPen>

#include "../../src/ui/ui.h"
#include "../../src/model/models.h"
#include "../../src/handlers/handlers.h"


class BaseTest : public QObject {
    Q_OBJECT

protected:
    using Text = Poppler::TextBox;
    using TextPtr = shared_ptr<Text>;
    using TextList = QList<TextPtr>;

    const QString base = "/home/roma/ws/C/scrs/e_reader/tests/samples";
    DocView* view;
    BaseDocument* doc;
    DocHandler* controller;
    QUrl* path;

protected:
    void compareRect(void);
    float getCurrentLocation(int loc);

public:
    virtual ~BaseTest();
};


class SinglePageViewTest : public BaseTest {
    Q_OBJECT

public:
    explicit SinglePageViewTest();
    ~SinglePageViewTest() override;

private slots:
    void testSceneRect(void);
    void testScrollingDown(void);
    void testScrollingUp(void);
    void testTextBoxes(void);
    void testDoubleClicked(void);
};


class MultPageViewTest : public BaseTest {
    Q_OBJECT

public:
    MultPageViewTest();
    ~MultPageViewTest() override;

private slots:
    void testSceneRect(void);
    void testScrollingDown(void);
    void testTextBoxes(void);
};
