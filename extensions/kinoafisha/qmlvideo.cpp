#include "qmlvideo.h"
#include <QDebug>
#include <QTime>

QAtomicInt QMLVideo::_instanceCount = 0;

QMLVideo::QMLVideo(QDeclarativeItem *parent) :
    QDeclarativeItem(parent),
    _phononVideoWidget(new Phonon::VideoWidget()),
    _mediaObject(new Phonon::MediaObject(this)),
    _audioOutput(new Phonon::AudioOutput(Phonon::VideoCategory, this)),
    _proxy(new QGraphicsProxyWidget(this)),
    _autoPlay(false),
    _startTime(0),
    _endTime(0),
    _playFinished(true),
    _paused(false)
{
    QTime progressTime;
    qDebug() << "Creating QMLVideo...";
    progressTime.start();
    qDebug() << "1. Creating audio patch...";
    Phonon::createPath(_mediaObject, _audioOutput);
    qDebug() << "2. Creating video patch...";
    Phonon::createPath(_mediaObject, _phononVideoWidget);

    qDebug() << "3. Set tick interval 100...";
    _mediaObject->setTickInterval(100);

    qDebug() << "4. Creationg mediaObject conection...";
    connect(_mediaObject, SIGNAL(finished()), this, SLOT(onPlayFinished()));
    connect(_mediaObject, SIGNAL(stateChanged(Phonon::State,Phonon::State)), this, SLOT(mediaObjectStateChanged(Phonon::State,Phonon::State)));
    connect(_mediaObject, SIGNAL(tick(qint64)), this, SLOT(mediaTick(qint64)));

    qDebug() << "5. Move video widget to 0, 0...";
    _phononVideoWidget->move(0, 0);

    qDebug() << "6. Set video widget to graphics proxy...";
    _proxy->setWidget(_phononVideoWidget);
    qDebug() << "Creating QMLVideo finished in " << progressTime.elapsed();
    qDebug() << "QMLVideo count " << _instanceCount.fetchAndAddRelaxed(1) + 1;
}

QMLVideo::~QMLVideo()
{
    _phononVideoWidget->deleteLater();
    qDebug() << "QMLVideo count " << _instanceCount.fetchAndAddRelaxed(-1) - 1;
//    _mediaObject->deleteLater();
//    _audioOutput->deleteLater();
}

void QMLVideo::play()
{
    if ((_mediaObject->currentSource().url().isEmpty() && !isPlaying())
            || _paused)
        return;
    qDebug() << "QMLVideo play";
    _playFinished = false;
    _paused = false;
    _mediaObject->play();
}

void QMLVideo::pause()
{
    _mediaObject->pause();
    _paused = true;
}

void QMLVideo::stop()
{
    qDebug() << "QMLVideo stop";
    _mediaObject->stop();
}

void QMLVideo::seak(const qint64 time)
{
    qDebug() << "QMLVideo seak to " << time;
    if (_mediaObject->isSeekable())
        _mediaObject->seek(time);
}

void QMLVideo::setSource(const QUrl &source)
{
    qDebug() << "QMLVideo set source " << source.toString();
    _mediaObject->setCurrentSource(Phonon::MediaSource(source));
    if (_autoPlay)
        play();
}

void QMLVideo::setStartTime(const qint64 time)
{
    if (_startTime == time)
        return;
    _startTime = time;
    emit startTimeChanged();
}

void QMLVideo::setEndTime(const qint64 time)
{
    if (_endTime == time)
        return;
    _endTime = time;
    emit endTimeChanged();
}

void QMLVideo::setBrightness(const qreal brightness)
{
    if (_brightness == brightness)
        return;
    _brightness = brightness;
    _phononVideoWidget->setBrightness(brightness);
    emit brightnessChanged();
}

void QMLVideo::setContrast(const qreal contrast)
{
    if (_contrast == contrast)
        return;
    _contrast = contrast;
    _phononVideoWidget->setContrast(contrast);
    emit contrastChanged();
}

void QMLVideo::setSaturation(const qreal saturation)
{
    if (_saturation == saturation)
        return;
    _saturation = saturation;
    _phononVideoWidget->setSaturation(saturation);
    emit saturationChanged();
}

void QMLVideo::setHue(const qreal hue)
{
    if (_hue == hue)
        return;
    _hue = hue;
    _phononVideoWidget->setHue(hue);
    emit hueChanged();
}

QUrl QMLVideo::source() const
{
    return _mediaObject->currentSource().url();
}

void QMLVideo::setWidth(qreal width)
{
    QSize size = _phononVideoWidget->size();
    if (size.width() == width)
        return;
    size.setWidth(width);
    _phononVideoWidget->resize(size);
    emit widthChanged();
}

void QMLVideo::setHeight(qreal height)
{
    QSize size = _phononVideoWidget->size();
    if (size.height() == height)
        return;
    size.setHeight(height);
    _phononVideoWidget->resize(size);
    emit heightChanged();
}

qreal QMLVideo::width() const
{
    return _phononVideoWidget->size().width();
}

qreal QMLVideo::height() const
{
    return _phononVideoWidget->size().height();
}

void QMLVideo::setX(qreal x)
{
    if (_proxy->x() == x)
        return;
    _proxy->setX(x);
    emit xChanged();
}

void QMLVideo::setY(qreal y)
{
    if (_proxy->y() == y)
        return;
    _proxy->setY(y);
    emit yChanged();
}

void QMLVideo::setZValue(qreal z)
{
    if (_proxy->zValue() == z)
        return;
    _proxy->setZValue(z);
    emit zChanged();
}

qreal QMLVideo::x() const
{
    return _proxy->x();
}

qreal QMLVideo::y() const
{
    return _proxy->y();
}

qreal QMLVideo::zValue() const
{
    return _proxy->zValue();
}

bool QMLVideo::autoPlay() const
{
    return _autoPlay;
}

void QMLVideo::setAutoPlay(const bool autoPlay)
{
    if (_autoPlay == autoPlay)
        return;
    _autoPlay = autoPlay;
    if (_autoPlay)
        play();
    emit autoPlayChanged();
}

void QMLVideo::setMute(const bool mute)
{
    if (_audioOutput->isMuted() == mute)
        return;
    _audioOutput->setMuted(mute);
    emit muteChanged();
}

bool QMLVideo::mute() const
{
    return _audioOutput->isMuted();
}

bool QMLVideo::isPlaying() const
{
    if (_mediaObject->state() == Phonon::PlayingState || _mediaObject->state() == Phonon::LoadingState || _mediaObject->state() == Phonon::BufferingState)
        return true;
    return false;
}

void QMLVideo::mediaObjectStateChanged(Phonon::State newstate, Phonon::State oldstate)
{
    qDebug() << "QMLVideo for source " << source().toString() << " state changed on " << oldstate << " to " << newstate;
    if (newstate == Phonon::PlayingState)
    {
        if (_startTime > 0)
            _mediaObject->seek(_startTime);
        _phononVideoWidget->setBrightness(_brightness);
        _phononVideoWidget->setContrast(_contrast);
        _phononVideoWidget->setSaturation(_saturation);
        _phononVideoWidget->setHue(_hue);
    }
    else if (oldstate == Phonon::PlayingState &&
             (newstate == Phonon::PausedState || newstate == Phonon::StoppedState) &&
             _playFinished)
    {
        emit finished();
    }
}

void QMLVideo::mediaTick(qint64 currentTime)
{
    if (_endTime > 0 && currentTime >= _endTime)
    {
        _playFinished = true;
        _mediaObject->pause();
    }
}

void QMLVideo::onPlayFinished()
{
    qDebug() << "QMLVideo for source " << source().toString() << " finished";
    _playFinished = true;
}
