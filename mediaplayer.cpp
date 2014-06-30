#define DEFAULT_EXTENSION_PLAY_TIME 10000

#include "mediaplayer.h"
#include "mdvplayerapplication.h"
#include <QGraphicsObject>
#include <QDesktopWidget>
#include <QApplication>
#include <QDebug>
#include <QKeyEvent>
#include <QDeclarativeItem>

QAtomicInt MediaPlayer::_instanceCount = 0;
QDeclarativeView * MediaPlayer::_videoInstance = NULL;

MediaPlayer::MediaPlayer(QWidget *parent) :
    QWidget(parent),
    _currentScreenNumber(0),
    _stackedLayout(new QStackedLayout(this)),
    _videoPlayer(new QDeclarativeView(this)),
    _extenstionPlayTimer(new QTimer(this)),
    _isPlaying(false)
{
    QTime _progressTime;
    qDebug() << "Creating new media player...";
    _progressTime.start();
//    _videoPlayer->setResizeMode(QDeclarativeView::SizeRootObjectToView);
    _videoPlayer->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    //Если не загружен плейер, то заргузим его
    if (!_videoInstance)
    {
        _videoInstance = new QDeclarativeView();
        _videoInstance->setSource(QUrl::fromLocalFile(QCoreApplication::applicationDirPath() + "/qml/VideoPlayer.qml"));
        QDeclarativeItem * rootItem = qobject_cast<QDeclarativeItem *>(_videoInstance->rootObject());
        rootItem->setWidth(QApplication::desktop()->screenGeometry(1).width());
        rootItem->setHeight(QApplication::desktop()->screenGeometry(1).height());

    }
    connect(_videoInstance->rootObject(), SIGNAL(finished(int)), this, SLOT(playVideoFinished(int)));
    _videoPlayer->setScene(_videoInstance->scene());


    _extenstionPlayTimer->setSingleShot(true);

    connect(_extenstionPlayTimer, SIGNAL(timeout()), this, SIGNAL(finished()));
    setLayout(_stackedLayout);
    qDebug() << "Creating new media player finished in " << _progressTime.elapsed() << " msec";
    qDebug() << "Media player object count " << _instanceCount.fetchAndAddRelease(1) + 1;
}

MediaPlayer::~MediaPlayer()
{
    qDebug() << "Media player object count " << _instanceCount.fetchAndAddRelease(-1) - 1;
}

void MediaPlayer::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
        emit escKeyPressed();
    QWidget::keyReleaseEvent(event);
}

void MediaPlayer::fillLayout()
{
    qDebug() << "Fill media player layout...";
    //Отчистим лайаут
    for (int i = 0; i < _stackedLayout->count(); i++)
    {
        _stackedLayout->removeWidget(_stackedLayout->widget(i));
    }
    //Создадим лайаут, содержащий плейер и расширения
    _stackedLayout->addWidget(_videoPlayer);
    ExtensionsEngine *extEngine = MDVPlayerApplication::getExtensionEngine();
    if (extEngine)
    {
        QList<AbstarctExtension *> extensions = extEngine->getExtensions();
        foreach (AbstarctExtension *extension, extensions)
        {
            if (extension && extension->getIsEnabled())
            {
                //Создадим строку содержащую название расширения и текущее расширение экрана
                QDesktopWidget *pDesktop = QApplication::desktop();
                QRect screenGeometry = pDesktop->screenGeometry(_currentScreenNumber);
                QString uniqueExtensionName = QString("%0-%1x%2").arg(extension->getIdentificationName())
                        .arg(screenGeometry.width())
                        .arg(screenGeometry.height());
                if (_extensionsViews.contains(uniqueExtensionName))
                {
                    _stackedLayout->addWidget(_extensionsViews[uniqueExtensionName]);
                }
                else
                {
                    QDeclarativeView *newGraphicsView = new QDeclarativeView(this);
                    newGraphicsView->setScene(extension->getSceneToShow(screenGeometry.width(), screenGeometry.height()));
                    _extensionsViews[uniqueExtensionName] = newGraphicsView;
                    _stackedLayout->addWidget(newGraphicsView);
                }
            }
        }
    }
    else
    {
        qWarning() << "MediaPlayer::fillLayout() extEngine NULL";
    }
    qDebug() << "Fill media player layout finished";
}

void MediaPlayer::playVideoFinished(const int id)
{
    Q_UNUSED(id);
    emit finished();
}

void MediaPlayer::stop()
{
    _extenstionPlayTimer->stop();
    QMetaObject::invokeMethod(_videoInstance->rootObject(), "stop");
    _isPlaying = false;
}

void MediaPlayer::playMedia(Media *media)
{
    if (!media)
    {
        qWarning() << "MediaPlayer::playMedia() media NULL";
        _isPlaying = false;
        return;
    }
    qDebug() << "Media player layout count " << _stackedLayout->count();
    _isPlaying = true;
    //Перезаполним лайаут... Надо перезаполнять каждый раз, т.к. может измениться инфо об расширении
    fillLayout();
    qDebug() << "After fill layout, layout count " << _stackedLayout->count();
//    //Сохраним страую сцену
//    QGraphicsScene *oldScene = NULL;
//    if (_stackedLayout->currentIndex() > -1)
//    {
//        QDeclarativeView *dv = qobject_cast<QDeclarativeView *>(_stackedLayout->currentWidget());
//        if (dv)
//            oldScene = dv->scene();
//    }
    if (!media->getIsExtention())
    {
        qDebug() << "Play video...";
        //Если это не расширение, то просто покажем плейер и воспроизведем видео
        _stackedLayout->setCurrentWidget(_videoPlayer);
        //Поставим на паузу все расширения
        ExtensionsEngine *extEngine = MDVPlayerApplication::getExtensionEngine();
        if (extEngine)
        {
            QList<AbstarctExtension *> extensions = extEngine->getExtensions();
            foreach (AbstarctExtension *e, extensions)
            {
                e->pause();
            }
        }

//        //Если не загружен плейер, то заргузим его
//        if (_videoPlayer->source().isEmpty())
//        {
//            _videoPlayer->setSource(QUrl::fromLocalFile(QCoreApplication::applicationDirPath() + "/qml/VideoPlayer.qml"));
//            connect(_videoPlayer->rootObject(), SIGNAL(finished(int)), this, SLOT(playVideoFinished(int)));
//        }
        QObject *mediaObject = qobject_cast<QObject *>(media);
        if (!mediaObject)
        {
            qWarning() <<" MediaPlayer::playMedia() mediaObject  NULL";
            _isPlaying = false;
            return;
        }
        QMetaObject::invokeMethod(_videoInstance->rootObject(), "playMedia",
                                  Q_ARG(QVariant, QVariant::fromValue(mediaObject)));
    } else
    {
        qDebug() << "Play extension...";
        //Если расширение, то найдем в стакедлайауте виджет расширения и покажем его
        ExtensionsEngine *extEngine = MDVPlayerApplication::getExtensionEngine();
        if (!extEngine)
        {
            qWarning() <<" MediaPlayer::playMedia() extEngine  NULL";
            _isPlaying = false;
            return;
        }
        QString extensionName = media->getExtentionIdentificationName();
        AbstarctExtension *extension = extEngine->getExtension(extensionName);
        if (!extension)
        {
            qWarning() <<" MediaPlayer::playMedia() extension  NULL";
            _isPlaying = false;
            return;
        }
        qDebug() << "Found extesion widget...";
        //Создадим строку содержащую название расширения и текущее расширение экрана
        QDesktopWidget *pDesktop = QApplication::desktop();
        QRect screenGeometry = pDesktop->screenGeometry(_currentScreenNumber);
        QString uniqueExtensionName = QString("%0-%1x%2").arg(extension->getIdentificationName())
                .arg(screenGeometry.width())
                .arg(screenGeometry.height());
        QWidget *extensionWidget = qobject_cast<QWidget *>(_extensionsViews[uniqueExtensionName]);
        qDebug() << "Setting media player current widget to extension widget...";
        _stackedLayout->setCurrentWidget(extensionWidget);
        //Установим время, через которое должно закончиться отображение разширения и запустим таймер
        qint64 timeToShowExtension = media->getEndPositionMSecs() - media->getStartPositionMSecs();
        if (timeToShowExtension < 1)
            timeToShowExtension = DEFAULT_EXTENSION_PLAY_TIME;
        //Запустим расширение
        qDebug() << "Starting extension timer...";
        extension->play();
        _extenstionPlayTimer->setInterval(timeToShowExtension);
        _extenstionPlayTimer->start();
    }
    qDebug() << "Found new graphics scene...";
    //Получим новую сцену
    QGraphicsScene *newScene = NULL;
    if (_stackedLayout->currentIndex() > -1)
    {
        QDeclarativeView *dv = qobject_cast<QDeclarativeView *>(_stackedLayout->currentWidget());
        if (dv)
            newScene = dv->scene();
    }
//    //Если сцена изменилась
//    if (oldScene != newScene)
    qDebug() << "Emit graphics scene changed...";
        emit graphicSceneChanged(newScene);
}
