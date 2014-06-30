#ifndef MDVMAINWINDOW_H
#define MDVMAINWINDOW_H

#include <QMainWindow>
#include <mediatablemodel.h>
#include <QTimer>
#include <QPointer>
#include "aboutdialog.h"
#include "screentab.h"
#include "logdialog.h"

namespace Ui {
class MDVMainWindow;
}

class MDVMainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MDVMainWindow(QWidget *parent = 0);

    void loadProject(const QString filePatch, const bool autoStart = false);
    ~MDVMainWindow();
    
private slots:
    void screensCurrentTabChanged(const int index);
    ScreenTab *addScreenTab(Screen *screen = NULL);
    void removeScreenTab();
    void renameScreenTab();
    void showScreenTabContextMenu(const QPoint& pos);
    void setScreenTabName(const QString &newName);
    void newProject();
    bool saveProjectAs();
    bool saveProject();
    void loadProject();
    void projectDataChanged();
    void showExtensionsConfigDialog();
    void showLogDialog();
    void updateExtensionsTab();
    void showConfigDialog();

protected:
    void closeEvent(QCloseEvent * event);

private:
    void restoreWindowsState();
    void loadSettingsParams();
    bool saveProject(const QString &saveFileName);
    void stopAllPlaying();
    void prepareToClose();
    Ui::MDVMainWindow *ui;
    int _screenTabNumberToContextMenu;
    int _lastScreen;
    QMenu *_screenTabContextMenu;
    AboutDialog *_aboutDialog;
    QString _newProjectFileName;
    QTimer *_autosaveTimer;
    QPointer<LogDialog> _logDialog;
};

#endif // MDVMAINWINDOW_H
