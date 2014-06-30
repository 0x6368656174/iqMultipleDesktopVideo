#ifndef MEDIAPLAYER_H
#define MEDIAPLAYER_H

#include <QWidget>
#include <QDeclarativeView>
#include <QStackedLayout>
#include <QTimer>
#include <QAtomicInt>
#include "media.h"
#include "extensions/abstarctextension.h"

class MediaPlayer : public QWidget
{
    Q_OBJECT
public:
    explicit MediaPlayer(QWidget *parent = 0);

    ~MediaPlayer();

    inline void setCurrentScreenNumber(const int number) {_currentScreenNumber = number;}
    
public slots:
    void playMedia(Media * media);
    void stop();

signals:
    void finished();
    void graphicSceneChanged(QGraphicsScene *scene);
    void escKeyPressed();

private slots:
    void fillLayout();
    void playVideoFinished(const int id);

protected:
    void keyPressEvent(QKeyEvent *event);

private:
    int _currentScreenNumber;
    QStackedLayout *_stackedLayout;
    QDeclarativeView *_videoPlayer;
    static QDeclarativeView *_videoInstance;
    QTimer *_extenstionPlayTimer;
    QHash<QString, QGraphicsView *> _extensionsViews;
    bool _isPlaying;
    static QAtomicInt _instanceCount;
};

#endif // MEDIAPLAYER_H
