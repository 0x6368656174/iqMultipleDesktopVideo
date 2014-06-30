#ifndef DESKTOPMEDIAPLAYER_H
#define DESKTOPMEDIAPLAYER_H

#include "screen.h"
#include "mediaplayer.h"
#include <QObject>
#include <QPointer>
#include <QAtomicInt>

class DesktopMediaPlayer : public QObject
{
    Q_OBJECT
public:
    explicit DesktopMediaPlayer(QObject *parent = 0);

    ~DesktopMediaPlayer();
    
    inline void setScreen(Screen *screen) {_currentScreen = screen;}

    inline bool isPlaying() const {return _isPlaying;}

    bool isShowed() const;


public slots:
    bool play();
    void stop();

signals:
    void graphicSceneChanged(QGraphicsScene *scene);
    void escKeyPressed();

private slots:
    void onPlayFinished();
    bool playNextMedia();

private:
    QPointer<Screen> _currentScreen;
    bool _isPlaying;
    QPointer<Media> _currentPlayedMedia;
//    MediaPlayer *_mediaPlayer;
    QList<MediaPlayer *> _mediaPlayers;

    static QAtomicInt _instanceCount;
};

#endif // DESKTOPMEDIAPLAYER_H
