#define DEFAULT_EXTENSION_PLAY_TIME 10000

#include "desktopmediaplayer.h"
#include "mdvplayerapplication.h"
#include <QDesktopWidget>
#include <QApplication>
#include <QStackedLayout>
#include <QUrl>
#include <QDebug>

QAtomicInt DesktopMediaPlayer::_instanceCount = 0;

DesktopMediaPlayer::DesktopMediaPlayer(QObject *parent) :
    QObject(parent),
    _currentScreen(NULL),
    _isPlaying(false),
    _currentPlayedMedia(NULL)/*,
    _mediaPlayer(new MediaPlayer())*/
{
//    connect(_mediaPlayer, SIGNAL(finished()), this, SLOT(onPlayFinished()));
//    connect(_mediaPlayer, SIGNAL(graphicSceneChanged(QGraphicsScene*)), this, SIGNAL(graphicSceneChanged(QGraphicsScene*)));
//    connect(_mediaPlayer, SIGNAL(escKeyPressed()), this, SIGNAL(escKeyPressed()));

    qDebug() << "Created new desktop media player. Total player count " << _instanceCount.fetchAndAddRelaxed(1) + 1;
}

DesktopMediaPlayer::~DesktopMediaPlayer()
{
//    _mediaPlayer->deleteLater();
    qDeleteAll(_mediaPlayers);
    qDebug() << "Deleted desktop media player. Total player count " << _instanceCount.fetchAndAddRelaxed(-1) - 1;
}

bool DesktopMediaPlayer::play()
{
    //Создадим плееры
    bool needCoonnect = _mediaPlayers.count() == 0;
    for (int i = _mediaPlayers.count(); i < QApplication::desktop()->screenCount() - 1; i++)
    {
        MediaPlayer *mp = new MediaPlayer();
        _mediaPlayers << mp;
    }
    if (needCoonnect && _mediaPlayers.count() > 0)
    {
        connect(_mediaPlayers[0], SIGNAL(finished()), this, SLOT(onPlayFinished()));
        connect(_mediaPlayers[0], SIGNAL(graphicSceneChanged(QGraphicsScene*)), this, SIGNAL(graphicSceneChanged(QGraphicsScene*)));
    }

    //Настроим плэер
    if (!_currentScreen)
    {
        qWarning() << "DesktopMediaPlayer::play() _currentScreen NULL";
        return false;
    }
//    int screenNumber = _currentScreen->getNumberOfScreen();
    QDesktopWidget *pDesktop = QApplication::desktop();
//    if (screenNumber >= pDesktop->screenCount())
//        return false;
    int screenN = 0;
    if (_mediaPlayers.count() == 0)
        return false;
    foreach (MediaPlayer * player, _mediaPlayers)
    {
        screenN++;
        QRect screenGeometry = pDesktop->screenGeometry(screenN);
        qDebug() << "Showed media player widget with " << screenGeometry << " geometry";
        player->setGeometry(screenGeometry);
        player->setWindowState(Qt::WindowFullScreen);
        player->setCurrentScreenNumber(screenN);
        player->show();
    }
//    QRect screenGeometry = pDesktop->screenGeometry(screenNumber);
//    qDebug() << "Showed media player widget with " << screenGeometry << " geometry";
//    QWidget *screen = pDesktop->screen(screenNumber);
//    _mediaPlayer->setParent(screen);
//    _mediaPlayer->setGeometry(screenGeometry);
//    _mediaPlayer->setWindowState(Qt::WindowFullScreen);
//    _mediaPlayer->setCurrentScreenNumber(screenNumber);
//    _mediaPlayer->show();

    playNextMedia();
    return true;
}

bool DesktopMediaPlayer::isShowed() const
{
    if (_mediaPlayers.count() > 0)
        return _mediaPlayers[0]->isVisible();
    return false;
//    return _mediaPlayer->isVisible();
}

void DesktopMediaPlayer::stop()
{
    foreach (MediaPlayer *player, _mediaPlayers)
    {
        player->stop();
        player->hide();
    }
//    _mediaPlayer->stop();
//    _mediaPlayer->hide();
    _isPlaying = false;
    _currentPlayedMedia = NULL;
    if (_currentScreen)
        _currentScreen->setPlayedMedia(NULL);
    emit graphicSceneChanged(NULL);
}

bool DesktopMediaPlayer::playNextMedia()
{       
    _isPlaying = true;
    if (!_currentScreen)
    {
        qWarning() << "DesktopMediaPlayer::playNextMedia() _currentScreen NULL";
        _isPlaying = false;
        return false;
    }
    Media * nextMedia = _currentScreen->getNextMediaToPlay();
    if (!nextMedia)
    {
        qWarning() << "DesktopMediaPlayer::playNextMedia() nextMedia NULL";
        _isPlaying = false;
        return false;
    }
    _currentPlayedMedia = nextMedia;
//    _mediaPlayer->playMedia(nextMedia);
    foreach (MediaPlayer * player, _mediaPlayers)
    {
        player->playMedia(nextMedia);
    }
    _currentScreen->setPlayedMedia(nextMedia);
    return true;
}

void DesktopMediaPlayer::onPlayFinished()
{
    _currentScreen->decreaseCurrentMediaNumberToShow();
    if (_isPlaying)
        playNextMedia();
}
