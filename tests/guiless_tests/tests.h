#ifndef TEST_H
#define TEST_H

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QUrl>
#include <QtCore/QString>
#include <QtGui/QPixmap>
#include <QtTest/QtTest>

#include "../../src/model/models.h"


class PDFDocumentTest : public QObject {
    Q_OBJECT

    const QString base = "/home/roma/ws/C/scrs/e_reader/tests/samples";
    const QList<QUrl> paths = {
        QUrl(base + "/single_page.pdf"),
        QUrl(base + "/small_doc.pdf"),
        QUrl(base + "/med_doc.pdf"),
        QUrl(base + "/large_doc.pdf"),
        QUrl(base + "/huge_doc.pdf"),
    };
    QList<BaseDocument*> docs;
    int length;
    double baseDpi;

public:
     PDFDocumentTest();
     ~PDFDocumentTest();

private:
     bool sizesAlmostEqual(QSize f_size, QSize s_size);
     void test();

private slots:
     void testPath(void);
     void testName(void);
     void testResizing(void);
     void testBuild(void);
     void testSizes(void);
};

#endif // TEST_H
