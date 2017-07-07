#pragma once

#include <iostream>
using std::cout;
using std::endl;
#include <vector>
using std::vector;
#include <memory>
using std::shared_ptr;
using std::make_shared;

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


static const QString base = "/home/roma/ws/C/scrs/e_reader/tests/samples";
using DocPtr = shared_ptr<BaseDocument>;


class BaseTest : public QObject {
    Q_OBJECT

protected:
    using Text = Poppler::TextBox;
    using TextPtr = shared_ptr<Text>;
    using TextList = QList<TextPtr>;

    DocPtr doc;
    DocHandler* controller;

protected:
    void compareRect(void);
    double getCurrentLocation(int loc);

public:
    BaseTest(const QString& name);
    virtual ~BaseTest();
};


class SinglePageViewTest : public BaseTest {
    Q_OBJECT

public:
    explicit SinglePageViewTest(const QString& name);
    ~SinglePageViewTest() override;

private slots:
    void testSceneRect(void);
    void testScrollingDown(void);
    void testScrollingUp(void);
    void testTextBoxes(void);
    void testDoubleClicked(void);
    void testSearch(void);
};


class MultPageViewTest : public BaseTest {
    Q_OBJECT

public:
    MultPageViewTest(const QString& name);
    ~MultPageViewTest() override;

private slots:
    void testSceneRect(void);
    void testScrollingDown(void);
    void testTextBoxes(void);
    void testSearch(void);
};


class SceneTest : public QObject {
    Q_OBJECT

    SceneHandler* controller;
    shared_ptr<BaseDocument> doc;
    QGraphicsView view;

    One2One<DocScene, SceneHandler>* connector;

public:
    SceneTest();
    ~SceneTest() override;

private slots:
    void testDbClick(void);
};
