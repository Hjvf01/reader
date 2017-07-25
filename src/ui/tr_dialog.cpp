#include "dialogs.h"


TrDialog::TrDialog(QWidget *parent) : QDialog(parent) {
    layout.addWidget(&tr_field);
    layout.addWidget(&lookup_field);
    setLayout(&layout);
    layout.setSizeConstraint(QLayout::SetFixedSize);
    setModal(true);
}

TrDialog::~TrDialog() {}


void TrDialog::setTranslate(const QString &translate_results) {
    tr_field.setText(translate_results);
}


void TrDialog::setLookup(const QString &lookup_results) {
    lookup_field.setText(lookup_results);
}


void TrDialog::closeEvent(QCloseEvent *) {
    tr_field.setText("");
    lookup_field.setText("");
    emit closeDialog();
}
