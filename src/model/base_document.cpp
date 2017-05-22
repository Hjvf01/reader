#include "models.h"


BaseDocument::BaseDocument(QString p, QString n) :
        QObject(), t_path(p), t_name(n) {}

QString BaseDocument::name() const { return t_name; }
QString BaseDocument::path() const { return t_path; }

bool BaseDocument::operator ==(BaseDocument* other) { return this == other; }
bool BaseDocument::operator !=(BaseDocument* other) { return this != other; }
