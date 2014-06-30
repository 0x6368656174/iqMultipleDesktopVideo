#include "extensionsengine.h"
#include "extensions/kinoafisha/kinoafishaextension.h"
#include <QDebug>

QAtomicInt ExtensionsEngine::_instanceCount = 0;
ExtensionsEngine::ExtensionsEngine(QObject *parent) :
    QObject(parent)
{
    KinoafishaExtension *kinoafisha = new KinoafishaExtension(this);
    connect(kinoafisha, SIGNAL(dataChanged()), this, SIGNAL(dataChanged()));
    _extensions[kinoafisha->getIdentificationName()] = kinoafisha;

    qDebug() << "Created new extensions engine. Total engines " << _instanceCount.fetchAndAddRelease(1) + 1;
}

ExtensionsEngine::~ExtensionsEngine()
{
    qDebug() << "Deleted extension engine. Total engines " << _instanceCount.fetchAndAddRelease(-1) - 1;
}

QList<AbstarctExtension *> ExtensionsEngine::getExtensions() const
{
    QList<AbstarctExtension *> result;
    QHashIterator<QString, AbstarctExtension *> i (_extensions);
    while (i.hasNext())
    {
        i.next();
        result << i.value();
    }
    return result;
}

AbstarctExtension* ExtensionsEngine::getExtension(const QString &name) const
{
    return _extensions[name];
}
