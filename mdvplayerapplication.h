#ifndef MDVPLAYERAPPLICATION_H
#define MDVPLAYERAPPLICATION_H

#include <QStandardItemModel>
#include <QObject>
#include <QMutex>
#include <QTranslator>
#include "extensionsengine.h"

class MDVPlayerApplication : public QObject
{
    Q_OBJECT
public:
    explicit MDVPlayerApplication(QObject *parent = 0);

    ~MDVPlayerApplication();
    
    static ExtensionsEngine * getExtensionEngine();

    static void setExtensionEngine(ExtensionsEngine *engine);

    static MDVPlayerApplication * instance();

    static void printToLog(QtMsgType type, QString &msg);

    static QStandardItemModel * logModel() {return MDVPlayerApplication::_logMadel;}

    static void setDebug(const bool debug);

    static void reloadTranslations();

signals:
    void extensionEngineDataChanged() const;

private:
    static ExtensionsEngine * _extenstionEngine;
    
    static MDVPlayerApplication * _instance;

    static QStandardItemModel *_logMadel;

    static QMutex _logMutex;

    static bool _dubug;

    static QTranslator *_qtTranslator;
    static QTranslator *_appTranslator;
    static QTranslator *_qmlTranslator;
    static bool _translatorSetted;
};

#endif // MDVPLAYERAPPLICATION_H
