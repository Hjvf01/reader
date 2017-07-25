#include "models.h"


BaseDocument::BaseDocument(const QString& p, const QString& n) :
        QObject(), path(p), name(n) {}

QString BaseDocument::getName() const { return name; }
QString BaseDocument::getPath() const { return path; }

bool BaseDocument::operator ==(BaseDocument* other) { return this == other; }
bool BaseDocument::operator !=(BaseDocument* other) { return this != other; }
