#ifndef DIALOGS_H
#define DIALOGS_H


#include <vector>
using std::vector;

#include <QtWidgets/QDialog>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QAction>
#include <QtWidgets/QPushButton>

#include <QtCore/QString>
#include <QtCore/QtDebug>

#include <QtGui/QCloseEvent>




class TrDialog : public QDialog {
    Q_OBJECT

    QVBoxLayout layout;
    QLabel tr_field;
    QLabel lookup_field;

signals:
    void closeDialog(void);

public:
    TrDialog(QWidget* parent=nullptr);
    ~TrDialog() override;

    void setTranslate(const QString& translate_results);
    void setLookup(const QString& lookup_results);

protected:
    void closeEvent(QCloseEvent*) override;
};




class FindDialog : public QDialog {
    Q_OBJECT

    QLineEdit input_field;
    QPushButton find_btn;
    QHBoxLayout layout;

    QAction find_action;
    QAction close_action;

public:
    FindDialog(QWidget* parent=nullptr);
    ~FindDialog() override;

private:
    void initConnectors(void);

protected:
    void closeEvent(QCloseEvent*) override;

signals:
    void find(const QString& text);
    void closeDialog(void);
};




class HelpDialog : public QDialog {
    Q_OBJECT

    QVBoxLayout v_box;
    QLabel lbl;

public:
    HelpDialog();
    ~HelpDialog() = default;
};

#endif // DIALOGS_H
