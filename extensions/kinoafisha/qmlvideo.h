#ifndef QMLVIDEO_H
#define QMLVIDEO_H

#include <QDeclarativeItem>
#include <QGraphicsProxyWidget>
#include <phonon/VideoWidget>
#include <phonon/MediaObject>
#include <phonon/AudioOutput>
#include <QAtomicInt>

class QMLVideo : public QDeclarativeItem
{
    Q_OBJECT

    Q_PROPERTY(QUrl source READ source WRITE setSource)
    Q_PROPERTY(qreal width READ width WRITE setWidth NOTIFY widthChanged)
    Q_PROPERTY(qreal height READ height WRITE setHeight NOTIFY heightChanged)
    Q_PROPERTY(qreal x READ x WRITE setX NOTIFY xChanged)
    Q_PROPERTY(qreal y READ y WRITE setY NOTIFY yChanged)
    Q_PROPERTY(qreal z READ zValue WRITE setZValue NOTIFY zChanged)
    Q_PROPERTY(bool autoPlay READ autoPlay WRITE setAutoPlay NOTIFY autoPlayChanged)
    Q_PROPERTY(bool mute READ mute WRITE setMute NOTIFY muteChanged)
    Q_PROPERTY(qint64 startTime READ getStartTime WRITE setStartTime NOTIFY startTimeChanged)
    Q_PROPERTY(qint64 endTime READ getEndTime WRITE setEndTime NOTIFY endTimeChanged)
    Q_PROPERTY(qreal brightness READ getBrightness WRITE setBrightness NOTIFY brightnessChanged)
    Q_PROPERTY(qreal contrast READ getContrast WRITE setContrast NOTIFY contrastChanged)
    Q_PROPERTY(qreal hue READ getHue WRITE setHue NOTIFY hueChanged)
    Q_PROPERTY(qreal saturation READ getSaturation WRITE setSaturation NOTIFY saturationChanged)

public:
    explicit QMLVideo(QDeclarativeItem *parent = 0);
    ~QMLVideo();

    void setHeight(qreal height);
    void setWidth(qreal width);
    qreal width() const;
    qreal height() const;
    void setX(qreal x);
    void setY(qreal y);
    void setZValue(qreal z);
    qreal x() const;
    qreal y() const;
    qreal zValue() const;
    void setSource(const QUrl &source);
    QUrl source() const;
    bool autoPlay() const;
    void setAutoPlay(const bool autoPlay);
    bool mute() const;
    void setMute(const bool mute);
    Q_INVOKABLE bool isPlaying() const;
    void setStartTime(const qint64 time);
    void setEndTime(const qint64 time);
    void setBrightness(const qreal brightness);
    void setSaturation(const qreal saturation);
    void setContrast(const qreal contrast);
    void setHue(const qreal hue);
    inline qint64 getStartTime() const {return _startTime;}
    inline qint64 getEndTime() const {return _endTime;}
    inline qreal getBrightness() const {return _brightness;}
    inline qreal getContrast() const {return _contrast;}
    inline qreal getSaturation() const {return _saturation;}
    inline qreal getHue() const {return _hue;}

signals:
    void widthChanged();
    void heightChanged();
    void xChanged();
    void yChanged();
    void zChanged();
    void autoPlayChanged();
    void muteChanged();
    void startTimeChanged();
    void endTimeChanged();
    void brightnessChanged();
    void contrastChanged();
    void saturationChanged();
    void hueChanged();
    void finished();

public slots:
    void play();
    void stop();
    void pause();
    void seak(const qint64 time);

private slots:
    void mediaObjectStateChanged(Phonon::State newstate, Phonon::State oldstate);
    void mediaTick(qint64 currentTime);
    void onPlayFinished();

private:
    Phonon::VideoWidget *_phononVideoWidget;
    Phonon::MediaObject *_mediaObject;
    Phonon::AudioOutput *_audioOutput;
    QGraphicsProxyWidget *_proxy;

    bool _autoPlay;
    qreal m_width;
    bool m_mute;

    qreal _brightness;
    qreal _contrast;
    qreal _saturation;
    qreal _hue;
    qint64 _startTime;
    qint64 _endTime;

    static QAtomicInt _instanceCount;

    bool _playFinished;
    bool _paused;
};

#endif // QMLVIDEO_H
