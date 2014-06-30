#ifndef SAVELOADINTERFACE_H
#define SAVELOADINTERFACE_H

#include <QObject>
#include <QDomElement>
#include "screen.h"
#include "extensionsengine.h"

class SaveLoadInterface : public QObject
{
    Q_OBJECT
public:
    explicit SaveLoadInterface(QObject *parent = 0);

    bool saveScreen(const QString saveFileName, const Screen *screenToSave);
    bool saveProject(const QString saveFileName, const QList<const Screen *> screens, const ExtensionsEngine *extensions);

    Screen * loadScreen(const QString loadFileName) const;
    QList<Screen *> loadScreens(const QString loadFileName);
    ExtensionsEngine * loadExtensions(const QString loadFileName);
    
private:
    QDomElement createScreenElement(QDomDocument &owner, const Screen *screen) const;
    QDomElement createExtensionElement(QDomDocument &owner, const ExtensionsEngine *extensions) const;
    Screen * parseScreenElement(const QDomElement &screenElement) const;
    ExtensionsEngine * parseExtensionsElement(const QDomElement &extensionsElement) const;
};

#endif // SAVELOADINTERFACE_H
