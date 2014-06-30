#include "media.h"
#include "extensionsengine.h"
#include "mdvplayerapplication.h"
#include <QFile>

qint64 Media::_lastId = -1;

Media::Media(QObject *parent) :
    QObject(parent),
    _id(++Media::_lastId),
    _filePatch(""),
    _title(""),
    _numberToShow(-1),
    _randomWeigth(1),
    _startPositionMSecs(-1),
    _endPositionMSecs(-1),
    _brightness(0),
    _contrast(0),
    _hue(0),
    _saturation(0),
    _extentionIdentificationName(""),
    _startTime(QTime(0, 0)),
    _endTime(QTime(23, 59, 59)),
    _startDate(QDate(QDate::currentDate().year(), 1, 1)),
    _endDate(QDate(QDate::currentDate().year() + 2, 12, 31))
{
    connect(this, SIGNAL(titleChanged()), this, SIGNAL(dataChanged()));
}

void Media::setFilePatch(const QString &patch)
{
    if (_filePatch != patch)
    {
        _filePatch = patch;
        emit dataChanged();
        emit filePatchChanged();
    }
}

void Media::setNumberToShow(const int numberToShow)
{
    if (_numberToShow != numberToShow)
    {
        _numberToShow = numberToShow;
        emit dataChanged();
    }
}

void Media::setRandomWeigth(const int weigth)
{
    if (_randomWeigth != weigth)
    {
        _randomWeigth = weigth;
        emit dataChanged();
    }
}

void Media::setStartPositionMSecs(const qint64 msecs)
{
    if (_startPositionMSecs != msecs)
    {
        _startPositionMSecs = msecs;
        emit dataChanged();
    }
}

void Media::setEndPositionMSecs(const qint64 msecs)
{
    if (_endPositionMSecs != msecs)
    {
        _endPositionMSecs = msecs;
        emit dataChanged();
    }
}

void Media::setBrightness(const qreal brightness)
{
    if (_brightness != brightness)
    {
        _brightness = brightness;
        emit dataChanged();
    }
}

void Media::setContrast(const qreal contrast)
{
    if (_contrast != contrast)
    {
        _contrast = contrast;
        emit dataChanged();
    }
}

void Media::setHue(const qreal hue)
{
    if (_hue != hue)
    {
        _hue = hue;
        emit dataChanged();
    }
}

void Media::setSaturation(const qreal saturation)
{
    if (_saturation != saturation)
    {
        _saturation = saturation;
        emit dataChanged();
    }
}

void Media::setStartTime(const QTime &time)
{
    if (_startTime != time)
    {
        _startTime = time;
        emit dataChanged();
    }
}

void Media::setEndTime(const QTime &time)
{
    if (_endTime != time)
    {
        _endTime = time;
        emit dataChanged();
    }
}

void Media::setStartDate(const QDate &date)
{
    if (_startDate != date)
    {
        _startDate = date;
        emit dataChanged();
    }
}

void Media::setEndDate(const QDate &date)
{
    if (_endDate != date)
    {
        _endDate = date;
        emit dataChanged();
    }
}

QHash<QString, QVariant> Media::getParams() const
{
    QHash<QString, QVariant> result;
    result["file-patch"] = getFilePatch();
    result["title"] = getTitle();
    result["number-to-show"] = getNumberToShow();
    result["random-weigth"] = getRandomWeigth();
    result["start-position-milliseconds"] = getStartPositionMSecs();
    result["end-position-milliseconds"] = getEndPositionMSecs();
    result["brightness"] = getBrightness();
    result["contrast"] = getContrast();
    result["saturation"] = getSaturation();
    result["hue"] = getHue();
    result["extensionName"] = getExtentionIdentificationName();
    result["startTime"] = getStartTime();
    result["endTime"] = getEndTime();
    result["startDate"] = getStartDate();
    result["endDate"] = getEndDate();
    return result;
}

void Media::setParams(QHash<QString, QVariant> params)
{
    setFilePatch(params.value("file-patch", "").toString());
    setTitle(params.value("title", "").toString());
    setNumberToShow(params.value("number-to-show", -1).toInt());
    setRandomWeigth(params.value("random-weigth", 1).toInt());
    setStartPositionMSecs(params.value("start-position-milliseconds", -1).toULongLong());
    setEndPositionMSecs(params.value("end-position-milliseconds", -1).toULongLong());
    setBrightness(params.value("brightness", 0).toReal());
    setContrast(params.value("contrast", 0).toReal());
    setSaturation(params.value("saturation", 0).toReal());
    setHue(params.value("hue", 0).toReal());
    setExtentionIdentificationName(params.value("extensionName", "").toString());
    setStartTime(params.value("startTime", QTime(0, 0)).toTime());
    setEndTime(params.value("endTime", QTime(23, 59, 59)).toTime());
    setStartDate(params.value("startDate", QDate(QDate::currentDate().year(), 1, 1)).toDate());
    setEndDate(params.value("endDate", QDate(QDate::currentDate().year() + 2, 12, 31)).toDate());
}

bool Media::isValid() const
{
    if (!getIsExtention() && !QFile::exists(_filePatch))
        return false;
    if (getIsExtention())
    {
        ExtensionsEngine *extensionsEngine = MDVPlayerApplication::getExtensionEngine();
        if (extensionsEngine)
        {
            AbstarctExtension *mediaExtension = extensionsEngine->getExtension(_extentionIdentificationName);
            if (mediaExtension)
            {
                if (!mediaExtension->getIsEnabled())
                    return false;
            }
        }
    }
    return true;
}

bool Media::isEnabled() const
{
    if (_numberToShow == 0)
        return false;
    QDateTime currentDateTime = QDateTime::currentDateTime();
    if (_startTime > currentDateTime.time())
        return false;
    if (_endTime < currentDateTime.time())
        return false;
    if (_startDate > currentDateTime.date())
        return false;
    if (_endDate < currentDateTime.date())
        return false;
    return true;
}

void Media::setTitle(const QString &title)
{
    if (_title == title)
        return;
    _title = title;
    emit titleChanged();
}
