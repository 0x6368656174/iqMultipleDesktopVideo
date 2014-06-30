#include "saveloadinterface.h"
#include <QFile>
#include <QTextStream>
#include <QDomNamedNodeMap>

SaveLoadInterface::SaveLoadInterface(QObject *parent) :
    QObject(parent)
{
}

bool SaveLoadInterface::saveScreen(const QString saveFileName, const Screen *screenToSave)
{
    QDomDocument domDoc;
    QDomElement screenElement = createScreenElement(domDoc, screenToSave);
    domDoc.appendChild(screenElement);

    QFile file (saveFileName);
    if (!file.open(QIODevice::WriteOnly))
        return false;

    QTextStream out(&file);
    domDoc.save(out, 4);

    file.close();
    return true;
}

bool SaveLoadInterface::saveProject(const QString saveFileName, const QList<const Screen *> screens, const ExtensionsEngine *extensions)
{
    QDomDocument domDoc;
    QDomElement projectElement = domDoc.createElement("project");
    QDomElement screensListElement = domDoc.createElement("screens-list");
    foreach (const Screen *screen, screens)
    {
        QDomElement screenElement = createScreenElement(domDoc, screen);
        screensListElement.appendChild(screenElement);
    }
    projectElement.appendChild(screensListElement);
    QDomElement extensionsListElement = createExtensionElement(domDoc, extensions);
    projectElement.appendChild(extensionsListElement);

    domDoc.appendChild(projectElement);

    QFile file (saveFileName);
    if (!file.open(QIODevice::WriteOnly))
        return false;

    QTextStream out(&file);
    domDoc.save(out, 4);

    file.close();
    return true;
}

Screen * SaveLoadInterface::loadScreen(const QString loadFileName) const
{
    QFile file (loadFileName);
    if (!file.open(QIODevice::ReadOnly))
        return NULL;

    QDomDocument domDoc;
    if (!domDoc.setContent(&file))
    {
        file.close();
        return NULL;
    }
    file.close();

    QDomElement screenElement = domDoc.namedItem("screen").toElement();
    if (screenElement.isNull())
        return NULL;

    return parseScreenElement(screenElement);
}

ExtensionsEngine * SaveLoadInterface::loadExtensions(const QString loadFileName)
{
    QFile file (loadFileName);
    if (!file.open(QIODevice::ReadOnly))
        return NULL;

    QDomDocument domDoc;
    if (!domDoc.setContent(&file))
    {
        file.close();
        return NULL;
    }
    file.close();

    QDomElement projectElement = domDoc.namedItem("project").toElement();
    if (projectElement.isNull())
        return NULL;

    QDomElement extensionstElement = projectElement.namedItem("extensions").toElement();

    if (extensionstElement.isNull())
        return NULL;

    return parseExtensionsElement(extensionstElement);

}

QList<Screen *> SaveLoadInterface::loadScreens(const QString loadFileName)
{
    QList<Screen *> result;
    QFile file (loadFileName);
    if (!file.open(QIODevice::ReadOnly))
        return result;

    QDomDocument domDoc;
    if (!domDoc.setContent(&file))
    {
        file.close();
        return result;
    }
    file.close();

    QDomElement projectElement = domDoc.namedItem("project").toElement();
    if (projectElement.isNull())
        return result;

    QDomElement screensListElement = projectElement.namedItem("screens-list").toElement();

    if (screensListElement.isNull())
        return result;

    QDomNode screenNode = screensListElement.firstChild();
    while (!screenNode.isNull())
    {
        QDomElement screenElement = screenNode.toElement();
        if (screenElement.isNull())
            continue;

        result << parseScreenElement(screenElement);
        screenNode = screenNode.nextSibling();
    }

    result.removeAll(NULL);
    return result;
}

QDomElement SaveLoadInterface::createScreenElement(QDomDocument &owner, const Screen *screen) const
{
    QDomElement screenElement = owner.createElement("screen");
    screenElement.setAttribute("name", screen->getName());
    screenElement.setAttribute("random-enabled", QVariant(screen->getRandomEnabled()).toString());
    screenElement.setAttribute("screen-number", QString::number(screen->getNumberOfScreen()));

    MediaTableModel *screenModel = screen->getMediaModel();
    if (screenModel)
    {
        QDomElement mediaList = owner.createElement("media-list");
        for (int i = 0; i < screenModel->rowCount(); i++)
        {
            Media *currentMedia = screenModel->data(screenModel->index(i, 0), MediaTableModel::MediaPnt).value<Media *>();
            if (!currentMedia)
                continue;
            QDomElement mediaElement = owner.createElement("media");
            QHash<QString, QVariant> mediaParams = currentMedia->getParams();
            QHashIterator<QString, QVariant> mediaParamsIterator(mediaParams);
            while(mediaParamsIterator.hasNext())
            {
                mediaParamsIterator.next();
                QDomElement paramDomElement = owner.createElement(mediaParamsIterator.key());
                QDomText paramDomText = owner.createTextNode(mediaParamsIterator.value().toString());
                paramDomElement.appendChild(paramDomText);
                mediaElement.appendChild(paramDomElement);
            }
            mediaList.appendChild(mediaElement);
        }

        screenElement.appendChild(mediaList);
    }

    return screenElement;
}

QDomElement SaveLoadInterface::createExtensionElement(QDomDocument &owner, const ExtensionsEngine *extensions) const
{
    QDomElement extensionsElement = owner.createElement("extensions");
    if (extensions)
    {
        QList<AbstarctExtension *> extensionsList = extensions->getExtensions();
        foreach (AbstarctExtension* extension, extensionsList)
        {
            if (!extension)
                continue;
            QDomElement extensionElement = owner.createElement("extension");
            QDomElement nameElement = owner.createElement("name");
            QDomText nameText = owner.createTextNode(extension->getIdentificationName());
            nameElement.appendChild(nameText);
            extensionElement.appendChild(nameElement);
            QDomElement isEnableElement = owner.createElement("enabled");
            QDomText isEnableText = owner.createTextNode(QVariant(extension->getIsEnabled()).toString());
            isEnableElement.appendChild(isEnableText);
            extensionElement.appendChild(isEnableElement);
            QDomElement paramsElement = owner.createElement("params");
            QHash<QString, QVariant> params = extension->getParams();
            QHashIterator<QString, QVariant> i (params);
            while (i.hasNext())
            {
                i.next();
                QDomElement paramElement = owner.createElement(i.key());
                QDomText paramText = owner.createTextNode(i.value().toString());
                paramElement.appendChild(paramText);
                paramsElement.appendChild(paramElement);
            }
            extensionElement.appendChild(paramsElement);
            if (extension->getScreen())
            {
                QDomElement screenElement = createScreenElement(owner, extension->getScreen());
                extensionElement.appendChild(screenElement);
            }
            extensionsElement.appendChild(extensionElement);
        }
    }
    return extensionsElement;
}

Screen * SaveLoadInterface::parseScreenElement(const QDomElement &screenElement) const
{
    Screen * resultScreen = new Screen();
    resultScreen->setName(screenElement.attribute("name"));
    resultScreen->setNumberOfScreen(screenElement.attribute("screen-number").toInt());
    resultScreen->setRandomEnabled(QVariant(screenElement.attribute("random-enabled")).toBool());

    MediaTableModel *screenModel = resultScreen->getMediaModel();

    if (screenModel)
    {
        QDomElement mediaListElement = screenElement.namedItem("media-list").toElement();
        if (!mediaListElement.isNull())
        {
            QDomNode mediaNode = mediaListElement.firstChild();
            while (!mediaNode.isNull())
            {
                QDomElement mediaElement = mediaNode.toElement();
                if (mediaElement.isNull())
                {
                    mediaNode = mediaNode.nextSibling();
                    continue;
                }
                int newModelRow = screenModel->rowCount();
                screenModel->insertRow(newModelRow);
                Media * newMedia = screenModel->data(screenModel->index(newModelRow, 0), MediaTableModel::MediaPnt).value<Media *>();
                if (!mediaElement.isNull())
                {
                    QDomNode paramNode = mediaElement.firstChild();
                    QHash<QString, QVariant> params;
                    while (!paramNode.isNull())
                    {
                        QDomElement paramElement = paramNode.toElement();
                        if (!paramElement.isNull())
                            params[paramElement.tagName()] = paramElement.text();

                        paramNode = paramNode.nextSibling();
                    }
                    newMedia->setParams(params);
                }

                mediaNode = mediaNode.nextSibling();
            }
        }
    }
    return resultScreen;
}

ExtensionsEngine * SaveLoadInterface::parseExtensionsElement(const QDomElement &extensionsElement) const
{
    ExtensionsEngine * result = new ExtensionsEngine();
    QDomNode extensionNode = extensionsElement.firstChild();
    while (!extensionNode.isNull())
    {
        QDomElement extensionElement = extensionNode.toElement();
        if (extensionElement.isNull())
        {
            extensionNode = extensionNode.nextSibling();
            continue;
        }
        QDomElement nameElement = extensionElement.namedItem("name").toElement();
        if (nameElement.isNull())
        {
            extensionNode = extensionNode.nextSibling();
            continue;
        }
        AbstarctExtension * extension = result->getExtension(nameElement.text());
        if (!extension)
        {
            extensionNode = extensionNode.nextSibling();
            continue;
        }
        QDomElement enabledElement = extensionNode.namedItem("enabled").toElement();
        if (!enabledElement.isNull())
        {
            extension->setIsEnabled(QVariant(enabledElement.text()).toBool());
        }
        QDomElement paramsElement = extensionElement.namedItem("params").toElement();
        if (!paramsElement.isNull())
        {
            QDomNode paramNode = paramsElement.firstChild();
            QHash<QString, QVariant> params;
            while (!paramNode.isNull())
            {
                QDomElement paramElement = paramNode.toElement();
                if (!paramElement.isNull())
                    params[paramElement.tagName()] = paramElement.text();

                paramNode = paramNode.nextSibling();
            }
            extension->setParams(params);
        }
        QDomElement screenElement = extensionElement.namedItem("screen").toElement();
        if (!screenElement.isNull())
        {
            Screen *extScreen = parseScreenElement(screenElement);
            extension->setScreen(extScreen);
        }

        extensionNode = extensionNode.nextSibling();
    }
    return result;
}
