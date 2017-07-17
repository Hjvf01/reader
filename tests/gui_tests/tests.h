#pragma once

#include <iostream>
using std::cout;
using std::endl;
#include <vector>
using std::vector;
#include <memory>
using std::shared_ptr;
using std::make_shared;
#include <cmath>
using std::abs;

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


static const double EPSILON = 0.01;


bool almostEqual(const QRectF&, const QRectF&, double);


using Size = unsigned int;
using Indx = unsigned int;
using Indexes = vector<Indx>;


enum class VerbosityLevel {
    silent = 0,
    verbose = 1
};


class SceneTest : public QObject {
    Q_OBJECT

    SceneHandler* controller;
    shared_ptr<BaseDocument> doc;
    QGraphicsView view;
    VerbosityLevel level;

    One2One<DocScene, SceneHandler>* connector;

public:
    SceneTest(VerbosityLevel lvl=VerbosityLevel::silent);
    ~SceneTest() override;

private slots:
    void testDbClick(void);
};


using DocPtr = shared_ptr<BaseDocument>;
class BaseTest : public QObject {
    Q_OBJECT

protected:
    using Text = Poppler::TextBox;
    using TextPtr = shared_ptr<Text>;
    using TextList = QList<TextPtr>;

    DocPtr doc;
    DocHandler* controller;
    VerbosityLevel level;

protected:
    void compareRect(void);
    double getCurrentLocation(int loc);

public:
    BaseTest(const QString& name, VerbosityLevel lvl);
    virtual ~BaseTest();
};


class SinglePageViewTest : public BaseTest {
    Q_OBJECT

public:
    explicit SinglePageViewTest(const QString& name, VerbosityLevel lvl);
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
    MultPageViewTest(const QString& name, VerbosityLevel lvl);
    ~MultPageViewTest() override;

private:
    void locationLog(int);

private slots:
    void testSceneRect(void);
    void testScrollingDown(void);
    void testScrollingUp(void);
    void testGoToNext(void);
    void testGoToPrev(void);
    void testGoTo(void);
    void testSearch(void);

    void testResize(void);
};


class BaseDocWidgetTest : public QObject {
    Q_OBJECT

protected:
    DocWidgetHandler* controller;

public:
    BaseDocWidgetTest(const QString& name, VerbosityLevel lvl);
    virtual ~BaseDocWidgetTest();
};


class SingleDocWidgetTest : public BaseDocWidgetTest {
    Q_OBJECT

    QRectF base_size;

public:
    SingleDocWidgetTest(const QString& name, VerbosityLevel lvl);
    ~SingleDocWidgetTest() override;

private slots:
    void testZoomIn(void);
    void testZoomOut(void);
    void testScaleBox(void);
};


class MultDocWidgetTest : public BaseDocWidgetTest {
    Q_OBJECT

public:
    MultDocWidgetTest(const QString& name, VerbosityLevel lvl);
    ~MultDocWidgetTest() override;

private:
    bool compareIndexes(const Indexes& actual, const Indexes& expected) const;

private slots:
    void testFirstPage(void);
    void testPrevPage(void);
    void testNextPage(void);
    void testLastPage(void);
};
