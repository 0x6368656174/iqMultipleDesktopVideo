#include "mediasettingsgroupbox.h"
#include "ui_mediasettingsgroupbox.h"
#include "mdvplayerapplication.h"
#include <QTimer>
#include <phonon/AudioOutput>
#include <phonon/MediaObject>
#include <phonon/VideoWidget>
#include <QFile>

MediaSettingsGroupBox::MediaSettingsGroupBox(QWidget *parent) :
    QGroupBox(parent),
    ui(new Ui::MediaSettingsGroupBox),
    _currentMedia(NULL),
    _timer(new QTimer(this)),
    _oldPreviewVolumeLvl()
{
    ui->setupUi(this);

    ui->errorLabel->setHidden(true);

    _timer->setInterval(1000);
    connect(_timer, SIGNAL(timeout()), this, SLOT(onPreviewTimer()));

    ui->mediaSettinsSplitter->setStretchFactor(1, 1);
    ui->previerVideoPlayer->setMinimumSize(ui->playHorizontalLayout->minimumSize().width(), ui->playHorizontalLayout->minimumSize().width()/1.78);

    connect(ui->titleLineEdit, SIGNAL(textChanged(QString)), this, SLOT(setMediaTitle(QString)));

    connect(ui->numberToShowsSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setCurrentMediaNumbreToShow(int)));
    connect(ui->randomWeightSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setCurrentMediaRandomWeigth(int)));
    connect(ui->startPositionTimetimeEdit, SIGNAL(timeChanged(QTime)), this, SLOT(setCurrentMediaStartPostion(QTime)));
    connect(ui->endPositionTimeTimeEdit, SIGNAL(timeChanged(QTime)), this, SLOT(setCurrentMediaEndPosition(QTime)));

    connect(ui->brightnessDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setCurrentMediaBrightness(double)));
    connect(ui->contrastDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setCurrentMediaContrast(double)));
    connect(ui->saturationDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setCurrentMediaSaturation(double)));
    connect(ui->hueDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setCurrentMediaHue(double)));

    connect(ui->playPauseToolButton, SIGNAL(clicked()), this, SLOT(playPausePreview()));
    connect(ui->stopToolButton, SIGNAL(clicked()), this, SLOT(stopPreview()));
    connect(ui->previewMuteToolButton, SIGNAL(clicked()), this, SLOT(setMuteUnMute()));
    connect(ui->previerVideoPlayer, SIGNAL(finished()), this, SLOT(previewFinished()));
    connect(ui->previerVideoPlayer->mediaObject(), SIGNAL(metaDataChanged()), this, SLOT(previewMediaMetadataChanged()));
    connect(ui->previerVideoPlayer->audioOutput(), SIGNAL(volumeChanged(qreal)), this, SLOT(previewMediaVolumeLvlChanded(qreal)));

    connect(ui->brightnessDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setPreviewPlayerBrightness(double)));
    connect(ui->contrastDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setPreviewPlayerContrast(double)));
    connect(ui->saturationDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setPreviewPlayerSaturation(double)));
    connect(ui->hueDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setPreviewPlayerHue(double)));

    connect(ui->startTimeTimeEdit, SIGNAL(timeChanged(QTime)), this, SLOT(setStartTime(QTime)));
    connect(ui->endTimeTimeEdit, SIGNAL(timeChanged(QTime)), this, SLOT(setEndTime(QTime)));
    connect(ui->stardDateDateEdit, SIGNAL(dateChanged(QDate)), this, SLOT(setStartDate(QDate)));
    connect(ui->endDateDateEdit, SIGNAL(dateChanged(QDate)), this, SLOT(setEndDate(QDate)));

    connect(ui->currentTimeStartToolButton, SIGNAL(clicked()), this, SLOT(setCurrentMediaTime()));
    connect(ui->currentTimeEndToolButton, SIGNAL(clicked()), this, SLOT(setCurrentMediaTime()));

    setMedia(NULL);
    ui->mediaSettinsSplitter->setSizes(QList<int>() << 1 << 10000);


}

MediaSettingsGroupBox::~MediaSettingsGroupBox()
{
    delete ui;
}

void MediaSettingsGroupBox::setMedia(Media *media)
{
    _currentMedia = NULL;
    this->setEnabled(false);
    ui->playPauseToolButton->setEnabled(false);
    ui->stopToolButton->setEnabled(false);
    ui->previewSeekSlider->setMediaObject(NULL);
    ui->previewVolumeSlider->setAudioOutput(NULL);
    ui->mediaInfoTableWidget->clear();
    ui->mediaTabWidget->setTabEnabled(1, false);
    ui->fileNameLineEdit->clear();
    ui->titleLineEdit->clear();
    ui->numberToShowsSpinBox->setValue(-1);
    ui->randomWeightSpinBox->setValue(0);
    ui->startPositionTimetimeEdit->setTime(QTime(0, 0, 0, 0));
    ui->endPositionTimeTimeEdit->setTime(QTime(0, 0, 0, 0));
    ui->currentTimeLabel->setText("00:00:00");
    ui->playPauseToolButton->setIcon(QIcon(":/icons/control.png"));
    ui->previerVideoPlayer->stop();
    ui->brightnessDoubleSpinBox->setValue(0);
    ui->contrastDoubleSpinBox->setValue(0);
    ui->saturationDoubleSpinBox->setValue(0);
    ui->hueDoubleSpinBox->setValue(0);
    ui->brightnessDoubleSpinBox->setEnabled(false);
    ui->contrastDoubleSpinBox->setEnabled(false);
    ui->saturationDoubleSpinBox->setEnabled(false);
    ui->hueDoubleSpinBox->setEnabled(false);
    ui->startTimeTimeEdit->setTime(QTime(0, 0));
    ui->endTimeTimeEdit->setTime(QTime(23, 59, 59));
    ui->stardDateDateEdit->setDate(QDate(QDate::currentDate().year(), 1, 1));
    ui->endDateDateEdit->setDate(QDate(QDate::currentDate().year() + 2, 12, 31));
    ui->errorLabel->setHidden(true);
    if (media)
    {
        _currentMedia = media;
        if (media->getIsExtention())
        {
            ExtensionsEngine *extensionsEngine = MDVPlayerApplication::getExtensionEngine();
            QString extName = media->getExtentionIdentificationName();
            if (extensionsEngine)
            {
                AbstarctExtension * extension = extensionsEngine->getExtension(media->getExtentionIdentificationName());
                if (extension)
                    extName = extension->getName();
            }
            ui->fileNameLineEdit->setText(extName);
        } else
        {
            ui->fileNameLineEdit->setText(media->getFilePatch());
        }
        ui->titleLineEdit->setText(media->getTitle());
        ui->numberToShowsSpinBox->setValue(media->getNumberToShow());
        ui->randomWeightSpinBox->setValue(media->getRandomWeigth());
        if (!media->getIsExtention())
        {
            ui->brightnessDoubleSpinBox->setValue(media->getBrightness());
            ui->contrastDoubleSpinBox->setValue(media->getContrast());
            ui->saturationDoubleSpinBox->setValue(media->getSaturation());
            ui->hueDoubleSpinBox->setValue(media->getHue());
//#ifndef Q_OS_WIN
            ui->brightnessDoubleSpinBox->setEnabled(true);
            ui->contrastDoubleSpinBox->setEnabled(true);
            ui->saturationDoubleSpinBox->setEnabled(true);
            ui->hueDoubleSpinBox->setEnabled(true);
//#endif
        }

        QTime startTime = QTime(0, 0, 0, 0);
        if (media->getStartPositionMSecs() > -1)
            startTime = QTime(0, 0, 0, 0).addMSecs(media->getStartPositionMSecs());
        ui->startPositionTimetimeEdit->setTime(startTime);
        QTime endTime = QTime(0, 0, 0, 0);
        if (media->getEndPositionMSecs() > -1)
            endTime = QTime(0, 0, 0, 0).addMSecs(media->getEndPositionMSecs());
        ui->endPositionTimeTimeEdit->setTime(endTime);

        ui->startTimeTimeEdit->setTime(media->getStartTime());
        ui->endTimeTimeEdit->setTime(media->getEndTime());
        ui->stardDateDateEdit->setDate(media->getStartDate());
        ui->endDateDateEdit->setDate(media->getEndDate());

        this->setEnabled(true);
        if (!media->getIsExtention())
        {
            ui->previerVideoPlayer->load(media->getFilePatch());
            ui->playPauseToolButton->setEnabled(true);
            ui->stopToolButton->setEnabled(true);
            ui->previewGroupBox->setEnabled(true);
        }


        if (!media->getIsExtention())
        {
            //Обработае ошибки
            if (!QFile::exists(media->getFilePatch()))
            {
                ui->errorLabel->setText(QString("<div style=\"color:#FF6347\">%0</div>").arg(tr("File not found")));
                ui->errorLabel->setVisible(true);
                ui->previewGroupBox->setEnabled(false);
            }

            ui->previewSeekSlider->setMediaObject(ui->previerVideoPlayer->mediaObject());
            ui->previewVolumeSlider->setAudioOutput(ui->previerVideoPlayer->audioOutput());
        }
    }
}

void MediaSettingsGroupBox::previewFinished()
{
    ui->currentTimeLabel->setText("00:00:00");
    ui->playPauseToolButton->setIcon(QIcon(":/icons/control.png"));
}

void MediaSettingsGroupBox::previewMediaVolumeLvlChanded(const qreal newVolume)
{
    if (newVolume == 0)
    {
        ui->previewMuteToolButton->setIcon(QIcon(":/icons/speaker-volume-control-mute.png"));
    } else
    {
        ui->previewMuteToolButton->setIcon(QIcon(":/icons/speaker-volume.png"));
    }
}

void MediaSettingsGroupBox::setMuteUnMute()
{
    if (ui->previerVideoPlayer->audioOutput()->volume() != 0)
    {
        _oldPreviewVolumeLvl = ui->previerVideoPlayer->audioOutput()->volume();
        ui->previerVideoPlayer->audioOutput()->setVolume(0);
    } else
    {
        ui->previerVideoPlayer->audioOutput()->setVolume(_oldPreviewVolumeLvl);
    }
}

void MediaSettingsGroupBox::previewMediaMetadataChanged()
{
    //Заполним медиоинфо
    ui->mediaInfoTableWidget->clear();
    ui->mediaInfoTableWidget->setColumnCount(2);
    QMultiMap<QString, QString> mediaInfo = ui->previerVideoPlayer->mediaObject()->metaData();
    QMutableMapIterator<QString, QString> i (mediaInfo);
    int row = -1;
    while (i.hasNext())
    {
        row++;
        ui->mediaInfoTableWidget->setRowCount(row + 1);
        i.next();
        QTableWidgetItem *firstItem = new QTableWidgetItem(i.key());
        ui->mediaInfoTableWidget->setItem(row, 0, firstItem);
        QTableWidgetItem *secondItem = new QTableWidgetItem(i.value());
        ui->mediaInfoTableWidget->setItem(row, 1, secondItem);
    }
    QTableWidgetItem *firtsHeaderItem = new QTableWidgetItem(tr("Key"));
    ui->mediaInfoTableWidget->setHorizontalHeaderItem(0, firtsHeaderItem);
    QTableWidgetItem *secondHeaderItem = new QTableWidgetItem(tr("Value"));
    ui->mediaInfoTableWidget->setHorizontalHeaderItem(1, secondHeaderItem);
    ui->mediaInfoTableWidget->horizontalHeader()->setResizeMode(0, QHeaderView::Interactive);
    ui->mediaInfoTableWidget->horizontalHeader()->setResizeMode(1, QHeaderView::Stretch);
    if (row != -1)
        ui->mediaTabWidget->setTabEnabled(1, true);
}

void MediaSettingsGroupBox::setCurrentMediaNumbreToShow(const int number)
{
    if (!_currentMedia)
        return;
    _currentMedia->setNumberToShow(number);
}

void MediaSettingsGroupBox::setCurrentMediaRandomWeigth(const int weigth)
{
    if (!_currentMedia)
        return;
    _currentMedia->setRandomWeigth(weigth);
}

void MediaSettingsGroupBox::setCurrentMediaStartPostion(const QTime &time)
{
    if (!_currentMedia)
        return;
    qint64 msecs = qAbs(time.msecsTo(QTime(0, 0, 0, 0)));
    _currentMedia->setStartPositionMSecs(msecs);
}

void MediaSettingsGroupBox::setCurrentMediaEndPosition(const QTime &time)
{
    if (!_currentMedia)
        return;
    qint64 msecs = qAbs(time.msecsTo(QTime(0, 0, 0, 0)));
    _currentMedia->setEndPositionMSecs(msecs);
}

void MediaSettingsGroupBox::setMediaTitle(const QString &title)
{
    if (!_currentMedia)
        return;
    _currentMedia->setTitle(title);
}

void MediaSettingsGroupBox::setCurrentMediaBrightness(const double brightness)
{
    if (!_currentMedia)
        return;
    _currentMedia->setBrightness(brightness);
}

void MediaSettingsGroupBox::setCurrentMediaContrast(const double contrast)
{
    if (!_currentMedia)
        return;
    _currentMedia->setContrast(contrast);
}

void MediaSettingsGroupBox::setCurrentMediaSaturation(const double saturation)
{
    if (!_currentMedia)
        return;
    _currentMedia->setSaturation(saturation);
}

void MediaSettingsGroupBox::setCurrentMediaHue(const double hue)
{
    if (!_currentMedia)
        return;
    _currentMedia->setHue(hue);
}

void MediaSettingsGroupBox::setStartTime(const QTime &time)
{
    if (!_currentMedia)
        return;
    _currentMedia->setStartTime(time);
}

void MediaSettingsGroupBox::setEndTime(const QTime &time)
{
    if (!_currentMedia)
        return;
    _currentMedia->setEndTime(time);
}

void MediaSettingsGroupBox::setStartDate(const QDate &date)
{
    if (!_currentMedia)
        return;
    _currentMedia->setStartDate(date);
}

void MediaSettingsGroupBox::setEndDate(const QDate &date)
{
    if (!_currentMedia)
        return;
    _currentMedia->setEndDate(date);
}

void MediaSettingsGroupBox::setPreviewPlayerBrightness(const double brightness)
{
    Phonon::VideoWidget *videoWidget = const_cast<Phonon::VideoWidget *>(ui->previerVideoPlayer->videoWidget());
    videoWidget->setBrightness(brightness);
}

void MediaSettingsGroupBox::setPreviewPlayerContrast(const double contrast)
{
    Phonon::VideoWidget *videoWidget = const_cast<Phonon::VideoWidget *>(ui->previerVideoPlayer->videoWidget());
    videoWidget->setContrast(contrast);
}

void MediaSettingsGroupBox::setPreviewPlayerSaturation(const double saturation)
{
    Phonon::VideoWidget *videoWidget = const_cast<Phonon::VideoWidget *>(ui->previerVideoPlayer->videoWidget());
    videoWidget->setSaturation(saturation);
}

void MediaSettingsGroupBox::setPreviewPlayerHue(const double hue)
{
    Phonon::VideoWidget *videoWidget = const_cast<Phonon::VideoWidget *>(ui->previerVideoPlayer->videoWidget());
    videoWidget->setHue(hue);
}

void MediaSettingsGroupBox::playPausePreview()
{
    if (ui->previerVideoPlayer->isPlaying())
    {
        //Надо поставить на паузу
        ui->previerVideoPlayer->pause();
        ui->playPauseToolButton->setIcon(QIcon(":/icons/control.png"));
    } else
    {
        //Надо начать воспроизводить
        ui->previerVideoPlayer->play();
        _timer->start();
        ui->playPauseToolButton->setIcon(QIcon(":/icons/control-pause.png"));
    }
}

void MediaSettingsGroupBox::stopPreview()
{
    ui->previerVideoPlayer->stop();
    _timer->stop();
    ui->playPauseToolButton->setIcon(QIcon(":/icons/control.png"));
}

void MediaSettingsGroupBox::setCurrentMediaTime()
{
    QToolButton * senderToolButton = qobject_cast<QToolButton*>(sender());
    if (!senderToolButton)
        return;
    QTime currentMediaTime (0, 0, 0, 0);
    Phonon::MediaObject *previewPlayerMediaObject = ui->previerVideoPlayer->mediaObject();
    if (!previewPlayerMediaObject)
        return;
    currentMediaTime = currentMediaTime.addMSecs(previewPlayerMediaObject->currentTime());
    if (senderToolButton == ui->currentTimeEndToolButton)
    {
        ui->endPositionTimeTimeEdit->setTime(currentMediaTime);
    }
    else if (senderToolButton == ui->currentTimeStartToolButton)
    {
        ui->startPositionTimetimeEdit->setTime(currentMediaTime);
    }
}

void MediaSettingsGroupBox::onPreviewTimer()
{
    QTime currentMediaTime (0, 0, 0, 0);
    Phonon::MediaObject *previewPlayerMediaObject = ui->previerVideoPlayer->mediaObject();
    if (!previewPlayerMediaObject)
        return;
    currentMediaTime = currentMediaTime.addMSecs(previewPlayerMediaObject->currentTime());
    ui->currentTimeLabel->setText(currentMediaTime.toString("hh:mm:ss"));
}
