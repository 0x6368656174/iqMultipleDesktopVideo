#ifndef MEDIASETTINGSGROUPBOX_H
#define MEDIASETTINGSGROUPBOX_H

#include <QGroupBox>
#include <QPointer>
#include "media.h"

namespace Ui {
class MediaSettingsGroupBox;
}

class MediaSettingsGroupBox : public QGroupBox
{
    Q_OBJECT
    
public:
    explicit MediaSettingsGroupBox(QWidget *parent = 0);
    ~MediaSettingsGroupBox();
    
public slots:
    void setMedia(Media * media);

private slots:
    void setCurrentMediaNumbreToShow(const int number);
    void setCurrentMediaRandomWeigth(const int weigth);
    void setCurrentMediaStartPostion(const QTime &time);
    void setCurrentMediaEndPosition(const QTime &time);
    void setCurrentMediaBrightness(const double brightness);
    void setCurrentMediaContrast(const double contrast);
    void setCurrentMediaSaturation(const double saturation);
    void setCurrentMediaHue(const double hue);
    void setPreviewPlayerBrightness(const double brightness);
    void setPreviewPlayerContrast(const double contrast);
    void setPreviewPlayerSaturation(const double saturation);
    void setPreviewPlayerHue(const double hue);
    void setStartTime(const QTime &time);
    void setEndTime(const QTime &time);
    void setStartDate(const QDate &date);
    void setEndDate(const QDate &date);
    void setMediaTitle(const QString &title);
    void playPausePreview();
    void stopPreview();
    void setCurrentMediaTime();
    void onPreviewTimer();
    void previewMediaMetadataChanged();
    void previewMediaVolumeLvlChanded(const qreal newVolume);
    void setMuteUnMute();
    void previewFinished();

private:
    Ui::MediaSettingsGroupBox *ui;
    QPointer<Media> _currentMedia;
    QTimer *_timer;
    qreal _oldPreviewVolumeLvl;
};

#endif // MEDIASETTINGSGROUPBOX_H
