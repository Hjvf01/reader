#ifndef DIALOGS_H
#define DIALOGS_H

#include <vector>
using std::vector;

#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSizePolicy>

#include <QtCore/QString>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonValue>
#include <QtCore/QList>
#include <QtCore/QVariant>
#include <QtCore/QtDebug>

#include <QtGui/QCloseEvent>


class TrDialog : public QDialog {
    Q_OBJECT

    QVBoxLayout box;
    QLabel translate;
    QLabel dict;
    vector<QLabel*> pos;
    QVBoxLayout pos_box;

signals:
    void closeDialog(void);

public:
    TrDialog(QWidget* parent=nullptr) : QDialog(parent) {
        box.addWidget(&translate);
        box.addWidget(&dict);
        dict.setLayout(&pos_box);
        setLayout(&box);

        box.setSpacing(0);
        pos_box.setSpacing(0);
        setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        pos_box.setSizeConstraint(QLayout::SetFixedSize);
        box.setSizeConstraint(QLayout::SetFixedSize);
        setModal(true);
    }

    void setTranslate(const QJsonObject result) {
        if(result["text"].isArray()) {
            QList<QVariant> array = result["text"].toArray().toVariantList();
            translate.setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            translate.setText(
                "<html><h3>" + array[0].toString() + "</h3></html>"
            );
        }
    }

    QLabel* getPos(const QJsonObject pos) {
        QString out = "<html>";
        out += "<strong>" + pos["text"].toString() +
               ": [" + pos["ts"].toString() + "]" +
               "<br><small>(" + pos["pos"].toString() + ")</small></strong>";
        out += "\n<ul>";
        for(QJsonValue elem: pos["tr"].toArray()) {
            QJsonObject obj = elem.toObject();
            out += "<li>" + obj["text"].toString() + "</li>\n";
        }
        out += "</ul></html>";
        return new QLabel(out);
    }

    void setLookup(const QJsonObject result) {
        if(result["def"].isArray())
            for(QJsonValue elem: result["def"].toArray()) {
                QLabel* p = getPos(elem.toObject());
                p->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
                pos.push_back(p);
                pos_box.addWidget(p);
            }
    }

    ~TrDialog() {
        for(QLabel* p: pos) delete p;
    }

protected:
    void closeEvent(QCloseEvent* event) override {
        Q_UNUSED(event);

        for(QLabel* lbl: pos) lbl->setText("");
        for(QLabel* lbl: pos) pos_box.removeWidget(lbl);
        emit closeDialog();
    }
};

#endif // DIALOGS_H
