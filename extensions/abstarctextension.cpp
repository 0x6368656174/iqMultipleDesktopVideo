#include "abstarctextension.h"

AbstarctExtension::AbstarctExtension(QObject *parent) :
    QObject(parent),
    _enabled(false)
{
}

void AbstarctExtension::setIsEnabled(const bool enabled)
{
    if (_enabled == enabled)
        return;
    _enabled = enabled;
    emit dataChanged();
}
