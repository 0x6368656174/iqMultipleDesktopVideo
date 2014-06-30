#ifndef EXTENSIONSENGINE_H
#define EXTENSIONSENGINE_H

#include <QObject>
#include <QAtomicInt>
#include "extensions/abstarctextension.h"

class ExtensionsEngine : public QObject
{
    Q_OBJECT
public:
    explicit ExtensionsEngine(QObject *parent = 0);

    ~ExtensionsEngine();

    AbstarctExtension * getExtension (const QString & name) const;

    QList<AbstarctExtension *> getExtensions () const;

signals:
    void dataChanged() const;
    
private:
    QHash<QString, AbstarctExtension *> _extensions;

    static QAtomicInt _instanceCount;
};

#endif // EXTENSIONSENGINE_H
