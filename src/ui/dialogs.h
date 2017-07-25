#ifndef DIALOGS_H
#define DIALOGS_H


#include <vector>
using std::vector;

#include <QtWidgets/QDialog>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>
#include <QtWidgets/QVBoxLayout>

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





class HelpDialog : public QDialog {
    Q_OBJECT

    QVBoxLayout v_box;
    QLabel lbl;

public:
    HelpDialog() {
        setLayout(&v_box);
        v_box.addWidget(&lbl);
        lbl.setText(
            "<html>"
                "<h4>File Menu</h4><ul>"
                    "<li>Ctrl+O:\tOpen</li>"
                    "<li>Ctrl+P:\tPrint</li>"
                    "<li>Alt+Return:\tDocument Info</li>"
                    "<li>Ctrl+W:\tClose</li>"
                    "<li>Ctrl+Q:\tQuit</li>"
                "</ul>"
                "<h4>View Menu</h4><ul>"
                    "<li>Ctrl+=:\tZoom-in</li>"
                    "<li>Ctrl+-:\tZoom-out</li>"
                    "<li>Home:\tFirstPage</li>"
                    "<li>Ctrl+Left:\tNext Page</li>"
                    "<li>Ctrl+Right:\tPrevious Page</li>"
                    "<li>End:\tLast Page</li>"
                    "<li>Ctrl+F11:\tFull Screen</li>"
                "</ul>"
                "<h4>Info Menu</h4><ul>"
                    "<li>Ctrl+H:   Help</li>"
                "</ul>"
            "</html>"
        );
    }

    ~HelpDialog() = default;
};

#endif // DIALOGS_H
