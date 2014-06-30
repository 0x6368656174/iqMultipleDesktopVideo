#ifndef SCREENTAB_H
#define SCREENTAB_H

#include <QWidget>
#include <QItemSelection>
#include "screen.h"
#include "desktopmediaplayer.h"
#include "extensionsengine.h"

namespace Ui {
class ScreenTab;
}

class ScreenTab : public QWidget
{
    Q_OBJECT
    
public:
    explicit ScreenTab(QWidget *parent = 0);
    ~ScreenTab();    

    void getDataToSave(MediaTableModel *& screenModel, QHash<QString, QVariant> *screenParams);

    void setData(MediaTableModel *screenModel, const QHash<QString, QVariant> screenParams);

    void setScreen(Screen *screen);

    inline Screen* getScreen () const {return _screen;}

signals:
    void currentMediaChanged(Media * currentMedia);
    void tabNameChanged(const QString &newName);
    void dataChanged();
    void screenPointerChanged(Screen *newScreen);

public slots:
    void setTabName(const QString &name);
    void stopPlaying();
    void startPlaying();

private slots:
    void mediaListSelectedChanded(const QItemSelection & selected, const QItemSelection & deselected);
    void addMedia();
    void addExtension();
    void removeMedia();
    void moveMediaUp();
    void moveMediaDown();
    void startStopPlaying();
    void save();
    void load();
    void updateExtensions();
    void onCurrentDisplaySpinBoxChanged(const int screenNumber);
    void setSceneToPreview(QGraphicsScene * scene);

private:
    Ui::ScreenTab *ui;
    Media *_currentMedia;
    Screen *_screen;
    DesktopMediaPlayer *_desktopMediaPlayer;

    void setModel(MediaTableModel *mediaModel);

    int getCurrentMediaTableRow() const;
};

#endif // SCREENTAB_H
