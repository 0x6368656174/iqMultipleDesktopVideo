#include "mdvmainwindow.h"
#include "screentab.h"
#include "ui_mdvmainwindow.h"
#include "logdialog.h"
#include "settingsdefines.h"
#include <QFileDialog>
#include <QInputDialog>
#include <QCloseEvent>
#include <QMessageBox>
#include <QSettings>
#include <phonon/BackendCapabilities>
#include "extensionsconfigdialog.h"
#include "mdvplayerapplication.h"
#include "saveloadinterface.h"
#include "configdialog.h"

MDVMainWindow::MDVMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MDVMainWindow),
    _screenTabNumberToContextMenu(-1),
    _lastScreen(0),
    _aboutDialog(new AboutDialog(this)),
    _newProjectFileName(tr("New Project.mdv")),
    _autosaveTimer(new QTimer(this)),
    _logDialog(NULL)
{
    ui->setupUi(this);
    restoreWindowsState();
    loadSettingsParams();
    ui->mainSplitter->setStretchFactor(1, 1);

    //Настроим меню
    connect(ui->actionAboutQt, SIGNAL(triggered()), QCoreApplication::instance(), SLOT(aboutQt()));
    connect(ui->actionAbout, SIGNAL(triggered()), _aboutDialog, SLOT(exec()));

    //Настроим там с экранами
    ui->screensTabWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    _screenTabContextMenu = new QMenu(ui->screensTabWidget);
//    _screenTabContextMenu->addAction(ui->actionAddScreen);
//    _screenTabContextMenu->addAction(ui->actionRemoveScreen);
    _screenTabContextMenu->addAction(ui->actionRenameScreen);
//    connect(ui->actionAddScreen, SIGNAL(triggered()), this, SLOT(addScreenTab()));
//    connect(ui->actionRemoveScreen, SIGNAL(triggered()), this, SLOT(removeScreenTab()));
    connect(ui->actionRenameScreen, SIGNAL(triggered()), this, SLOT(renameScreenTab()));
    connect(ui->screensTabWidget, SIGNAL(currentChanged(int)), this, SLOT(screensCurrentTabChanged(int)));
    connect(ui->screensTabWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showScreenTabContextMenu(QPoint)));

    connect(ui->actionNewProject, SIGNAL(triggered()), this, SLOT(newProject()));
    connect(ui->actionSaveProjectAs, SIGNAL(triggered()), this, SLOT(saveProjectAs()));
    connect(ui->actionSaveProject, SIGNAL(triggered()), this, SLOT(saveProject()));
    connect(ui->actionOpenProject, SIGNAL(triggered()), this, SLOT(loadProject()));

    connect(ui->actionExtensions, SIGNAL(triggered()), this, SLOT(showExtensionsConfigDialog()));
    connect(ui->actionSettings, SIGNAL(triggered()), this, SLOT(showConfigDialog()));

    connect(ui->actionLog, SIGNAL(triggered()), this, SLOT(showLogDialog()));

    //Таймер автосохранения
    connect(_autosaveTimer, SIGNAL(timeout()), this, SLOT(saveProject()));

    //Законектим изменение энжайна расширений на слот изменения проекта
    connect(MDVPlayerApplication::instance(), SIGNAL(extensionEngineDataChanged()), this, SLOT(projectDataChanged()));

    ui->mainSplitter->setSizes(QList<int>() << 1 << 10000);

    newProject();

    qDebug() << "Available Mime Types:" << Phonon::BackendCapabilities::availableMimeTypes();
}

MDVMainWindow::~MDVMainWindow()
{
    qDebug() << "Call ~MDVMainWindow()...";
    if (_logDialog)
        _logDialog->close();
    delete ui;
}

void MDVMainWindow::showExtensionsConfigDialog()
{
    ExtensionsConfigDialog *dialog = new ExtensionsConfigDialog(this);
    dialog->setEngine(MDVPlayerApplication::getExtensionEngine());
    dialog->exec();
    dialog->deleteLater();
}

void MDVMainWindow::restoreWindowsState()
{
    QSettings settings;
    move(settings.value(SETTINGS_KEY_MAIN_WINDOW_POS).toPoint());
    resize(settings.value(SETTINGS_KEY_MAIN_WINDOW_SIZE).toSize());
    restoreState(settings.value(SETTINGS_KEY_MAIN_WINDOW_STATE).toByteArray());
    if (settings.value(SETTINGS_KEY_MAIN_WINDOW_MAXIMAZID).toBool())
        setWindowState(windowState() | Qt::WindowMaximized);
}

void MDVMainWindow::loadSettingsParams()
{
    QSettings settings;
    _autosaveTimer->setInterval(settings.value(SETTINGS_KEY_AUTOSAVE_INTERVAL, DEFAULT_SETTINGS_KEY_AUTOSAVE_INTERVAL).toInt()*60000);
    if (settings.value(SETTINGS_KEY_AUTOSAVE_ENABLED, DEFAULT_SETTINGS_KEY_AUTOSAVE_ENABLED).toBool())
    {
        _autosaveTimer->start();
    } else
    {
        _autosaveTimer->stop();
    }
}

void MDVMainWindow::showConfigDialog()
{
    ConfigDialog *cfgdlg = new ConfigDialog(this);
    cfgdlg->exec();
    cfgdlg->deleteLater();
    loadSettingsParams();
}

void MDVMainWindow::newProject()
{
    _lastScreen = 0;
    MDVPlayerApplication::setExtensionEngine(new ExtensionsEngine(this));
    connect(MDVPlayerApplication::getExtensionEngine(), SIGNAL(dataChanged()), this, SLOT(updateExtensionsTab()));
    setWindowFilePath(_newProjectFileName);
    setWindowModified(false);
    ui->actionSaveProject->setEnabled(false);
    //Удалим все вкладки, кроме c "+"
    for (int i = ui->screensTabWidget->count() - 1; i > -1; i--)
    {
        if (!ui->screensTabWidget->tabToolTip(i).isEmpty())
            continue;
        QWidget *tabWidget = ui->screensTabWidget->widget(i);
        ui->screensTabWidget->removeTab(i);
        tabWidget->deleteLater();
    }
    if (ui->screensTabWidget->count() == 0)
        addScreenTab();
    //Создадим экраны для расширений
    foreach(AbstarctExtension *extension, MDVPlayerApplication::getExtensionEngine()->getExtensions())
    {
        if (extension && extension->getScreen())
        {
            ScreenTab *tab = addScreenTab(extension->getScreen());
            connect(tab, SIGNAL(screenPointerChanged(Screen*)), extension, SLOT(setScreen(Screen*)));
        }
    }
    updateExtensionsTab();
}

void MDVMainWindow::updateExtensionsTab()
{
    //Пройдемся по всем табам
    for (int i = 0; i < ui->screensTabWidget->count(); i++)
    {
        ScreenTab *screenTabWidget = qobject_cast<ScreenTab *>(ui->screensTabWidget->widget(i));
        if (!screenTabWidget)
            continue;
        Screen *tabScreen = screenTabWidget->getScreen();
        if (!tabScreen)
            continue;
        //Пройдемся по всем расширениям и найдем то, кому принадлежит данный экран
        foreach (AbstarctExtension * extension, MDVPlayerApplication::getExtensionEngine()->getExtensions())
        {
            if (extension && extension->getScreen() == tabScreen)
            {
                //Если данное расширение включено, то включим данный таб, иначе выключим
                ui->screensTabWidget->setTabEnabled(i, extension->getIsEnabled());
            }
        }
    }
}

void MDVMainWindow::loadProject(const QString filePatch, const bool autoStart)
{
    if (!QFile::exists(filePatch))
    {
        qWarning() << "File \"" << filePatch << "\" not exist";
        return;
    }
    QTime loadTime;

    SaveLoadInterface saveLoadInt;

    qDebug() << "Loding screens...";
    loadTime.restart();
    //Подготовим лист из экранов
    QList<Screen *> screens = saveLoadInt.loadScreens(filePatch);
    qDebug() << "Loading screens finished in " << loadTime.elapsed() << " msec";
    if (screens.count() == 0)
        return;
    qDebug() << "Creating new project...";
    loadTime.restart();
//    newProject();

    //Удалим все вкладки, кроме c "+"
    for (int i = ui->screensTabWidget->count() - 1; i > -1; i--)
    {
        if (!ui->screensTabWidget->tabToolTip(i).isEmpty())
            continue;
        QWidget *tabWidget = ui->screensTabWidget->widget(i);
        ui->screensTabWidget->removeTab(i);
        tabWidget->deleteLater();
    }
    qDebug() << "Creating new project finished in " << loadTime.elapsed() << " msec";

    qDebug() << "Loading extensions...";
    loadTime.restart();
    //Сначало загрузим новый энжайн расширений
    ExtensionsEngine *newExtensionEngine = saveLoadInt.loadExtensions(filePatch);
    newExtensionEngine->setParent(this);
    if (!newExtensionEngine)
        newExtensionEngine = new ExtensionsEngine(this);
    MDVPlayerApplication::setExtensionEngine(newExtensionEngine);
    connect(MDVPlayerApplication::getExtensionEngine(), SIGNAL(dataChanged()), this, SLOT(updateExtensionsTab()));
    qDebug() << "Loading extensions finished in " << loadTime.elapsed() << " msec";

    qDebug() << "Creating extension tabs...";
    loadTime.restart();
    //Создадим экраны для расширений
    foreach(AbstarctExtension *extension, newExtensionEngine->getExtensions())
    {
        if (extension && extension->getScreen())
        {
            ScreenTab *tab = addScreenTab(extension->getScreen());
            connect(tab, SIGNAL(screenPointerChanged(Screen*)), extension, SLOT(setScreen(Screen*)));
        }
    }
    updateExtensionsTab();
    qDebug() << "Creating extensions tabs finished in " << loadTime.elapsed() << " msec";

    qDebug() << "Creating screens tabs...";
    loadTime.restart();
    //Потом загрузим новые данные для экранов
    int curTab = -1;
    foreach (Screen *screen, screens)
    {
        curTab++;
        if (curTab != 0)
            addScreenTab();
        ScreenTab *currentScreenTab = qobject_cast<ScreenTab *>(ui->screensTabWidget->widget(curTab));
        if (!currentScreenTab)
            continue;
        currentScreenTab->setScreen(screen);
    }
    qDebug() << "Creating screens tabs finished in " << loadTime.elapsed() << " msec";

    setWindowFilePath(filePatch);
    setWindowModified(false);
    ui->actionSaveProject->setEnabled(false);

    //Если автоматический старт, то
    if (autoStart)
    {
        for (int i = 0; i < ui->screensTabWidget->count(); i++)
        {
            ScreenTab *screenWidgetOnTab = qobject_cast<ScreenTab *>(ui->screensTabWidget->widget(i));
            if (screenWidgetOnTab)
            {
                Screen *screenOnTab = qobject_cast<Screen *>(screenWidgetOnTab->getScreen());
                if (screenOnTab && screenOnTab->getIsRealScreen())
                {
                    //Если это экран, то запустим его
                    screenWidgetOnTab->startPlaying();
                }
            }
        }
    }

}

void MDVMainWindow::loadProject()
{
    QString openFileName = QFileDialog::getOpenFileName(this, tr("Open Project"), QString(), tr("MDV (*.MDV)"));
    if (openFileName.isEmpty())
        return;
    loadProject(openFileName);
}

bool MDVMainWindow::saveProject()
{
    QString saveFileName = windowFilePath();
    if (saveFileName.isEmpty())
        return false;
    if (saveFileName != _newProjectFileName)
    {
        return saveProject(saveFileName);
    } else
    {
        return saveProjectAs();
    }
    return false;
}

bool MDVMainWindow::saveProjectAs()
{
    QString saveFileName = QFileDialog::getSaveFileName(this, tr("Save Project As"), QString(), tr("MDV (*.mdv)"));
    if (saveFileName.isEmpty())
        return false;
    return saveProject(saveFileName);
}

bool MDVMainWindow::saveProject(const QString &saveFileName)
{
    //Подготовим лист из экранов
    QList<const Screen *> screens;
    for (int i = 0; i < ui->screensTabWidget->count() - 1; i++)
    {
        ScreenTab *currentScrenTab = qobject_cast<ScreenTab *>(ui->screensTabWidget->widget(i));
        if (!currentScrenTab || !currentScrenTab->getScreen())
            continue;
        if (!currentScrenTab->getScreen()->getIsRealScreen())
            continue;
        screens << currentScrenTab->getScreen();
    }
    SaveLoadInterface saveLoadInt;
    if (!saveLoadInt.saveProject(saveFileName, screens, MDVPlayerApplication::getExtensionEngine()))
    {
        return false;
    }
    setWindowFilePath(saveFileName);
    setWindowModified(false);
    ui->actionSaveProject->setEnabled(false);
    return true;
}

void MDVMainWindow::setScreenTabName(const QString &newName)
{
    QWidget *senderWidget = qobject_cast<QWidget *>(sender());
    int tabNumber = ui->screensTabWidget->indexOf(senderWidget);
    ui->screensTabWidget->setTabText(tabNumber, newName);
}

void MDVMainWindow::screensCurrentTabChanged(const int index)
{
    ScreenTab *widgetOnClickTab = qobject_cast<ScreenTab *>(ui->screensTabWidget->widget(index));
    //Проверим, чтоб этот экран можно было изменять
    if (widgetOnClickTab)
    {
        Screen *currentScreen = qobject_cast<Screen *>(widgetOnClickTab->getScreen());
        if (!currentScreen)
            return;
        if (!currentScreen->getDeleteRenameEnabled())
        {
//            ui->actionRemoveScreen->setEnabled(false);
            ui->actionRenameScreen->setEnabled(false);
        } else
        {
//            ui->actionRemoveScreen->setEnabled(true);
            ui->actionRenameScreen->setEnabled(true);
        }
    }

    //Если это не вкладка с экраном, то это значит (+) и надо добавить новый экран
    if (!widgetOnClickTab)
        addScreenTab();
}

ScreenTab * MDVMainWindow::addScreenTab(Screen *screen)
{
    ScreenTab *newTab = new ScreenTab(ui->screensTabWidget);
    connect(newTab, SIGNAL(currentMediaChanged(Media*)), ui->mediaSettinsGroupBox, SLOT(setMedia(Media*)));
    connect(newTab, SIGNAL(tabNameChanged(QString)), this, SLOT(setScreenTabName(QString)));
    connect(newTab, SIGNAL(dataChanged()), this, SLOT(projectDataChanged()));

    _lastScreen++;
    int addScrenPosition = 0;
    if (!screen || screen->getDeleteRenameEnabled())
    {
        //Найдем индекс таба с плюсом
        int plusTabIndex = 0;
        for (int i = 0; i < ui->screensTabWidget->count(); i++)
        {
            if (!ui->screensTabWidget->tabToolTip(i).isEmpty())
            {
                plusTabIndex = i;
                break;
            }
        }
        //Вставим строку
        addScrenPosition = plusTabIndex;
        ui->screensTabWidget->insertTab(plusTabIndex, newTab, tr("Screen %0").arg(_lastScreen));
    } else
    {
        addScrenPosition = ui->screensTabWidget->count();
        ui->screensTabWidget->insertTab(ui->screensTabWidget->count(), newTab, screen->getName());
    }
    if (screen)
    {
        if (!screen->getIcon().isNull())
            ui->screensTabWidget->setTabIcon(addScrenPosition, screen->getIcon());
        newTab->setScreen(screen);
    }
    ui->screensTabWidget->setCurrentIndex(addScrenPosition);
    projectDataChanged();
    return newTab;
}

void MDVMainWindow::projectDataChanged()
{
    setWindowModified(true);
    ui->actionSaveProject->setEnabled(true);
}

void MDVMainWindow::removeScreenTab()
{
    //Найдем индекс таба с плюсом
    int plusTabIndex = 0;
    for (int i = 0; i < ui->screensTabWidget->count(); i++)
    {
        if (!ui->screensTabWidget->tabToolTip(i).isEmpty())
        {
            plusTabIndex = i;
            break;
        }
    }
    if (_screenTabNumberToContextMenu > -1)
    {
        if (_screenTabNumberToContextMenu != 1 || plusTabIndex != 0)
        {
            if (_screenTabNumberToContextMenu -1 != plusTabIndex)
            {
                ui->screensTabWidget->setCurrentIndex(_screenTabNumberToContextMenu - 1);
            } else
            {
                ui->screensTabWidget->setCurrentIndex(_screenTabNumberToContextMenu - 2);
            }
        }
        QWidget *_tabWidget = ui->screensTabWidget->widget(_screenTabNumberToContextMenu);
        ui->screensTabWidget->removeTab(_screenTabNumberToContextMenu);
        _tabWidget->deleteLater();
    } else
    {
        ui->screensTabWidget->setCurrentIndex(ui->screensTabWidget->currentIndex() - 1);
        QWidget *_tabWidget = ui->screensTabWidget->currentWidget();
        ui->screensTabWidget->removeTab(ui->screensTabWidget->currentIndex());
        _tabWidget->deleteLater();
    }
    projectDataChanged();
}

void MDVMainWindow::renameScreenTab()
{
    bool ok = false;
    QString currentName = "";
    if (_screenTabNumberToContextMenu > -1)
    {
        currentName = ui->screensTabWidget->tabText(_screenTabNumberToContextMenu);
    } else
    {
        currentName = ui->screensTabWidget->tabText(ui->screensTabWidget->currentIndex());
    }
    QString newName = QInputDialog::getText(this, tr("New name"), tr("Input new name"), QLineEdit::Normal, currentName, &ok);
    if (!ok)
        return;
    ui->screensTabWidget->setTabText(_screenTabNumberToContextMenu, newName);
    ScreenTab * currentTab = qobject_cast<ScreenTab *>(ui->screensTabWidget->widget(_screenTabNumberToContextMenu));
    if (!currentTab)
        currentTab = qobject_cast<ScreenTab *>(ui->screensTabWidget->currentWidget());
    if (currentTab)
        currentTab->setTabName(newName);
}

void MDVMainWindow::showScreenTabContextMenu(const QPoint &pos)
{
    _screenTabNumberToContextMenu = ui->screensTabWidget->tabBar()->tabAt(pos);
    //Найдем индекс таба с плюсом
    int plusTabIndex = 0;
    for (int i = 0; i < ui->screensTabWidget->count(); i++)
    {
        if (!ui->screensTabWidget->tabToolTip(i).isEmpty())
        {
            plusTabIndex = i;
            break;
        }
    }
    if (_screenTabNumberToContextMenu == plusTabIndex || _screenTabNumberToContextMenu == -1)
    {
        _screenTabNumberToContextMenu = -1;
        return;
    }
    //Проверим, чтоб этот экран можно было изменять
    ScreenTab *currentTab = qobject_cast<ScreenTab *>(ui->screensTabWidget->widget(_screenTabNumberToContextMenu));
    if (!currentTab)
        return;
    Screen *currentScreen = qobject_cast<Screen *>(currentTab->getScreen());
    if (!currentScreen)
        return;
    if (!currentScreen->getDeleteRenameEnabled())
    {
//        ui->actionRemoveScreen->setEnabled(false);
        ui->actionRenameScreen->setEnabled(false);
    } else
    {
//        ui->actionRemoveScreen->setEnabled(true);
        ui->actionRenameScreen->setEnabled(true);
    }
    //Вызовем контекстное меню
    _screenTabContextMenu->exec(ui->screensTabWidget->mapToGlobal(pos));
    _screenTabNumberToContextMenu = -1;
}

void MDVMainWindow::closeEvent(QCloseEvent *event)
{
    QSettings settings;
    settings.setValue(SETTINGS_KEY_MAIN_WINDOW_SIZE, size());
    settings.setValue(SETTINGS_KEY_MAIN_WINDOW_POS, pos());
    settings.setValue(SETTINGS_KEY_MAIN_WINDOW_STATE, saveState());
    settings.setValue(SETTINGS_KEY_MAIN_WINDOW_MAXIMAZID, isMaximized());

    if (!isWindowModified())
    {
        prepareToClose();
        event->accept();
    } else
    {
        QMessageBox::StandardButton questionResult = QMessageBox::question(this, tr("The project has benn modified"),
                                                                           tr("The project has been modified, save changes?"),
                                                                           QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,
                                                                           QMessageBox::Yes);
        if (questionResult == QMessageBox::Cancel)
            event->ignore();

        if (questionResult == QMessageBox::No)
        {
            prepareToClose();
            event->accept();
        }

        if (questionResult == QMessageBox::Yes)
        {
            if (saveProject())
            {
                prepareToClose();
                event->accept();
            } else
            {
                event->ignore();
            }
        }
    }
    QMainWindow::closeEvent(event);
}

void MDVMainWindow::prepareToClose()
{
    stopAllPlaying();
    if (_logDialog)
    {
        _logDialog->close();
        _logDialog = NULL;
    }
}

void MDVMainWindow::stopAllPlaying()
{
    for (int i = 0; i < ui->screensTabWidget->count(); i++)
    {
        ScreenTab *tab = qobject_cast<ScreenTab *>(ui->screensTabWidget->widget(i));
        if (tab)
            tab->stopPlaying();
    }
}

void MDVMainWindow::showLogDialog()
{
    if (!_logDialog)
    {
        _logDialog = new LogDialog();
        _logDialog->setAttribute(Qt::WA_DeleteOnClose);
    }
    _logDialog->show();
    _logDialog->raise();
    _logDialog->activateWindow();
    _logDialog->resize(_logDialog->size().width(), _logDialog->size().height() + 1);
}
