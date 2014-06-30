#include "screen.h"
#include <QDebug>

QAtomicInt Screen::_instanceCount = 0;

Screen::Screen(QObject *parent) :
    QObject(parent),
    _mediaModel(NULL),
    _name(""),
    _randomEnabled(false),
    _numberOfScreen(0),
    _currentMediaToPlay(NULL),
    _isRealScreen(true),
    _deleteRenameEnabled(true),
    _icon(QIcon())
{
    setMediaModel(new MediaTableModel(this));

    qDebug() << "Createn new screen. Total screen number " << _instanceCount.fetchAndAddRelease(1) + 1;
}

Screen::~Screen()
{
    qDebug() << "Deleted screen. Total screen number " << _instanceCount.fetchAndAddRelease(-1) - 1;
}

void Screen::setMediaModel(MediaTableModel *model)
{
    if (model != _mediaModel)
    {
        if (_mediaModel)
            _mediaModel->deleteLater();
        _mediaModel = model;
        _mediaModel->setParent(this);
        connect(_mediaModel, SIGNAL(dataToSaveChanged()), this, SIGNAL(dataToSaveChanged()));
        emit mediaModelChanged(_mediaModel);
        emit dataToSaveChanged();
    }
}

void Screen::setName(const QString &name)
{
    if (name != _name)
    {
        _name = name;
        emit nameChanged(_name);
        emit dataToSaveChanged();
    }
}

void Screen::setRandomEnabled(const bool enabled)
{
    if (enabled != _randomEnabled)
    {
        _randomEnabled = enabled;
        emit randomEnabledChanged(_randomEnabled);
        emit dataToSaveChanged();
    }
}

void Screen::setNumberOfScreen(const int number)
{
    if (number != _numberOfScreen)
    {
        _numberOfScreen = number;
        emit numberOfScreenChanged(number);
        emit dataToSaveChanged();
    }
}

Media * Screen::getNextMediaToPlay(const QList<Media *> ignoredMediaList) const
{
    QStringList mediaListFilePatch;
    foreach (Media * media, ignoredMediaList)
    {
        if (media)
            mediaListFilePatch << media->getFilePatch();
    }
    qDebug() << "Find next media to play whith ingored media " << mediaListFilePatch << "...";
    if (!_mediaModel || _mediaModel->rowCount() < 1)
        return NULL;
    if (!_randomEnabled)
    {
        _currentMediaToPlay = _mediaModel->findNextMedia(_currentMediaToPlay, ignoredMediaList);
    } else
    {
        _currentMediaToPlay = _mediaModel->findNextRandomMedia(ignoredMediaList);
    }
    if (_currentMediaToPlay)
    {
        qDebug() << "Next media found " << _currentMediaToPlay->getFilePatch();
    } else
    {
        qDebug() << "Next media not found";
    }
    return _currentMediaToPlay;
}

void Screen::decreaseCurrentMediaNumberToShow()
{
    if (!_currentMediaToPlay)
        return;
    int currentNumberToShow = _currentMediaToPlay->getNumberToShow();
    if (currentNumberToShow > 0)
        _currentMediaToPlay->setNumberToShow(currentNumberToShow - 1);
}

void Screen::setPlayedMedia(const Media *media)
{
    if (!_mediaModel)
        return;
    _mediaModel->setCurrentPlayedMedia(media);
}

Media * Screen::getMedia(const qint64 id) const
{
    if (!_mediaModel)
        return NULL;
    return _mediaModel->getMedia(id);
}
