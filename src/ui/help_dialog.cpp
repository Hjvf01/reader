#include "dialogs.h"


HelpDialog::HelpDialog() : QDialog() {
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
