#include <QtGui/QKeySequence>

#include "dialogs.h"


FindDialog::FindDialog(QWidget *parent) : QDialog(parent) {
    find_btn.setIcon(QIcon(":/find.png"));
    find_btn.setFlat(true);

    find_action.setShortcut(QKeySequence(Qt::Key_Enter));
    close_action.setShortcut(QKeySequence(Qt::Key_Escape));

    addAction(&find_action);
    addAction(&close_action);

    layout.addWidget(&input_field);
    layout.addWidget(&find_btn);
    setLayout(&layout);
    setModal(true);

    setWindowTitle("Find");

    initConnectors();
}

FindDialog::~FindDialog() {}


void FindDialog::initConnectors() {
    connect(
        &find_btn, &QPushButton::pressed,
        [this]() { emit this->find(input_field.text()); }
    );

    connect(
        &find_action, &QAction::triggered,
        [this](bool) { emit this->find(input_field.text()); }
    );

    connect(
        &close_action, &QAction::triggered,
        [this](bool) { this->close(); }
    );
}


void FindDialog::closeEvent(QCloseEvent *) {
    input_field.setText("");
    emit closeDialog();
}
