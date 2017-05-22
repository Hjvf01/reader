#include "tests.h"


TranslatorDialogTest::TranslatorDialogTest() {
    dialog = new TranslatorDialog("foreign ford", "native word");
    dialog->show();
}

TranslatorDialogTest::~TranslatorDialogTest() { delete dialog; }
