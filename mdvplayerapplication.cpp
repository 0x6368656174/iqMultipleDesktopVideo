#include "mdvplayerapplication.h"
#include <QDateTime>
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>
#include <QDebug>
#include <QSettings>
#include <QLocale>
#include <QLibraryInfo>
#include <QCoreApplication>
#include "settingsdefines.h"

ExtensionsEngine * MDVPlayerApplication::_extenstionEngine = NULL;
MDVPlayerApplication * MDVPlayerApplication::_instance = NULL;
QStandardItemModel * MDVPlayerApplication::_logMadel = new QStandardItemModel();
bool MDVPlayerApplication::_dubug = false;
QMutex MDVPlayerApplication::_logMutex;
QTranslator *MDVPlayerApplication::_qtTranslator = new QTranslator();
QTranslator *MDVPlayerApplication::_appTranslator = new QTranslator();
QTranslator *MDVPlayerApplication::_qmlTranslator = new QTranslator();
bool MDVPlayerApplication::_translatorSetted = false;

MDVPlayerApplication::MDVPlayerApplication(QObject *parent) :
    QObject(parent)
{
}

MDVPlayerApplication::~MDVPlayerApplication()
{
    qDebug() << "Call ~MDVPlayerApplication()";
}

MDVPlayerApplication* MDVPlayerApplication::instance()
{
    if (!_instance)
    {
        _instance = new MDVPlayerApplication();
        _logMadel->setColumnCount(2);
        _logMadel->setHorizontalHeaderLabels(QStringList() << tr("Date Time") << tr("Message"));
    }
    return _instance;
}

ExtensionsEngine * MDVPlayerApplication::getExtensionEngine()
{
    return MDVPlayerApplication::_extenstionEngine;
}

void MDVPlayerApplication::setDebug(const bool debug)
{
    MDVPlayerApplication::_dubug = debug;
}

void MDVPlayerApplication::setExtensionEngine(ExtensionsEngine *engine)
{
    if (MDVPlayerApplication::_extenstionEngine)
    {
        MDVPlayerApplication::_extenstionEngine->disconnect();
        MDVPlayerApplication::_extenstionEngine->deleteLater();
    }
    MDVPlayerApplication::_extenstionEngine = engine;
    _extenstionEngine->setParent(MDVPlayerApplication::instance());
    connect(engine, SIGNAL(dataChanged()), MDVPlayerApplication::instance(), SIGNAL(extensionEngineDataChanged()));
    emit MDVPlayerApplication::instance()->extensionEngineDataChanged();
}

void MDVPlayerApplication::printToLog(QtMsgType type, QString &msg)
{
    if (type == QtDebugMsg && !_dubug)
        return;
    _logMutex.lock();
    QFile file(QCoreApplication::applicationDirPath() + "/log.txt");
    file.open(QIODevice::Append | QIODevice::Text);
    QTextStream out(&file);
    out << QDateTime::currentDateTime().toString() << "\n";
    switch (type) {
    case QtDebugMsg:
        out << tr("Debug: %0").arg(msg) << "\n\n";
        break;
    case QtWarningMsg:
        out << tr("Warning: %0").arg(msg) << "\n\n";
        break;
    case QtCriticalMsg:
        out << tr("Critical: %0").arg(msg) << "\n\n";
        break;
    case QtFatalMsg:
        out << tr("Fatal: %0").arg(msg) << "\n\n";
        break;
    }
    file.close();

    QStandardItemModel *model = MDVPlayerApplication::_logMadel;
    if (!model)
    {
        _logMutex.unlock();
        return;
    }
    model->setRowCount(model->rowCount() + 1);
    QStandardItem *dateTimeItem = new QStandardItem(QDateTime::currentDateTime().toString());
    QStandardItem *textItem = new QStandardItem();
    switch (type) {
    case QtDebugMsg:
        dateTimeItem->setBackground(QColor("white"));
        textItem->setText(tr("Debug: %0").arg(msg));
        textItem->setBackground(QColor("white"));
        break;
    case QtWarningMsg:
        dateTimeItem->setBackground(QColor("yellow"));
        textItem->setText(tr("Warning: %0").arg(msg));
        textItem->setBackground(QColor("yellow"));
        break;
    case QtCriticalMsg:
        dateTimeItem->setBackground(QColor("red"));
        textItem->setText(tr("Critical: %0").arg(msg));
        textItem->setBackground(QColor("red"));
        break;
    case QtFatalMsg:
        dateTimeItem->setBackground(QColor("darkRed"));
        textItem->setText(tr("Fatal: %0").arg(msg));
        textItem->setBackground(QColor("darkRed"));
        _logMutex.unlock();
        abort();
    }
    model->setItem(model->rowCount() - 1, 0, dateTimeItem);
    model->setItem(model->rowCount() - 1, 1, textItem);
    _logMutex.unlock();
}

void MDVPlayerApplication::reloadTranslations()
{
    //Загрузим трансляции
    QSettings settings;
    QString localeName = settings.value(SETTINGS_KEY_LANGUAGE, QLocale::system().name()).toString();

    _qtTranslator->load("qt_" + localeName,
                        QLibraryInfo::location(QLibraryInfo::TranslationsPath));

    QString appTransFP = QCoreApplication::applicationDirPath() + "/mdplayer_" + localeName;
    if (QFile::exists(appTransFP + ".qm"))
        _appTranslator->load(appTransFP);

    QString qmlTransFP = QCoreApplication::applicationDirPath() + "/mdplayer_qml_" + localeName;
    if (QFile::exists(qmlTransFP + ".qm"))
    {
        _qmlTranslator->load(qmlTransFP);
    }

    if (!_translatorSetted)
    {
        QCoreApplication *a = QCoreApplication::instance();
        a->installTranslator(_qtTranslator);
        a->installTranslator(_appTranslator);
        a->installTranslator(_qmlTranslator);
        _translatorSetted = true;
    }
}
