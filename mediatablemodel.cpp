#include "mediatablemodel.h"
#include "mdvplayerapplication.h"
#include <QFileInfo>
#include <QTime>
#include <QColor>
#include <QBrush>
#include <QFile>
#include <QDebug>

QAtomicInt MediaTableModel::_instanceCount = 0;

MediaTableModel::MediaTableModel(QObject *parent) :
    QAbstractTableModel(parent),
    _currentPlayedMedia(NULL)
{
    qDebug() << "Created new media table model. Total model count " << _instanceCount.fetchAndAddRelaxed(1) + 1;
}

MediaTableModel::~MediaTableModel()
{
    qDebug() << "Deleted media table model. Total model count " << _instanceCount.fetchAndAddRelaxed(-1) - 1;
}

QVariant MediaTableModel::data(const QModelIndex &index, int role) const
{
    if (index.column() >= ENDColumn || index.row() >= rowCount() || index.column() < 0 || index.row() < 0)
        return QVariant();
    Columns column = static_cast<Columns>(index.column());
    Media * mediaObject = _mediaObjects[index.row()];
    if (role == Qt::DisplayRole)
    {
        switch (column)
        {
        case FilePatch:
        {
            if (!mediaObject->getIsExtention())
            {
                QFileInfo fi (mediaObject->getFilePatch());
            return fi.fileName();
            } else
            {
                ExtensionsEngine *extensionsEngine = MDVPlayerApplication::getExtensionEngine();
                if (!extensionsEngine)
                    return mediaObject->getExtentionIdentificationName();
                AbstarctExtension *mediaExtension = extensionsEngine->getExtension(mediaObject->getExtentionIdentificationName());
                if (!mediaExtension)
                    return mediaObject->getExtentionIdentificationName();
                return mediaExtension->getName();
            }
            break;
        }
        case NumberToShow:
            if (mediaObject->getNumberToShow() == -1)
            {
                return QChar(8734); //знак бесконечности
            } else
            {
                return mediaObject->getNumberToShow();
            }
            break;
        case RandomWeigth:
            return mediaObject->getRandomWeigth();
            break;
        default:
            return QVariant();
            break;
        }
    }
    else if (role == Qt::ToolTipRole)
    {
        switch (column)
        {
        case FilePatch:
            return tr("File patch: %0").arg(mediaObject->getFilePatch());
            break;
        case NumberToShow:
            return tr("Number to show");
            break;
        case RandomWeigth:
            return tr("Random weigth");
            break;
        default:
            return QVariant();
            break;
        }
    }
    else if (role == Qt::StatusTipRole)
    {
        return mediaObject->getFilePatch();
    }
    else if (role == MediaPnt)
    {
        return QVariant::fromValue(mediaObject);
    }
    else if (role == ExtensionIdentificationName)
    {
        return mediaObject->getExtentionIdentificationName();
    }
    else if (role == ExtensionName)
    {
        ExtensionsEngine *extensionsEngine = MDVPlayerApplication::getExtensionEngine();
        if (!extensionsEngine)
            return mediaObject->getExtentionIdentificationName();
        AbstarctExtension *mediaExtension = extensionsEngine->getExtension(mediaObject->getExtentionIdentificationName());
        if (!mediaExtension)
            return mediaObject->getExtentionIdentificationName();
        return mediaExtension->getName();
    }
    else if (role == Qt::BackgroundRole)
    {
        if (mediaObject == _currentPlayedMedia)
            return QBrush(QColor("#00FF00"));
        if (!mediaObject->isValid())
            return QBrush(QColor("#FF6347"));
        if (!mediaObject->isEnabled())
            return QBrush(QColor("#BEBEBE"));
    }
    return QVariant();
}

bool MediaTableModel::insertRows(int row, int count, const QModelIndex &parent)
{
    emit beginInsertRows(parent, row, row + count - 1);
    for (int i = 0; i < count; i++)
    {
        Media * newMedia = new Media(this);
        connect(newMedia, SIGNAL(dataChanged()), this, SLOT(childDataChanded()));
        _mediaObjects.insert(row, newMedia);
        _mediaObjectsHash[newMedia->getId()] = newMedia;
    }
    emit endInsertRows();
    emit dataToSaveChanged();
    return true;
}

bool MediaTableModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if (row >= _mediaObjects.size() || row < 0)
        return false;
    emit beginRemoveRows(parent, row, row + count - 1);
    for(int i = row + count - 1; i > row - 1; i--)
    {
        Media *currentMedia = _mediaObjects[i];
        _mediaObjects.removeAt(i);
        if (currentMedia)
        {
            _mediaObjectsHash.remove(currentMedia->getId());
            if (_currentPlayedMedia == currentMedia)
                _currentPlayedMedia = NULL;
            currentMedia->deleteLater();
        }
    }
    emit endRemoveRows();
    emit dataToSaveChanged();
    return true;
}

bool MediaTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.column() >= ENDColumn || index.row() >= rowCount() || index.column() < 0 || index.row() < 0)
        return false;
    Columns column = static_cast<Columns>(index.column());
    Media * mediaObject = _mediaObjects[index.row()];
    if (role == Qt::EditRole)
    {
        switch (column)
        {
        case FilePatch:
            mediaObject->setFilePatch(value.toString());
            break;
        case NumberToShow:
            mediaObject->setNumberToShow(value.toInt());
            break;
        case RandomWeigth:
            mediaObject->setNumberToShow(value.toInt());
            break;
        default:
            break;
        }
    }
    else if (role == ExtensionIdentificationName)
    {
        mediaObject->setExtentionIdentificationName(value.toString());
    }
    emit dataChanged(this->index(index.row(), 0), this->index(index.row(), columnCount() - 1));
    emit dataToSaveChanged();
    return true;
}

void MediaTableModel::childDataChanded()
{
    Media * senderMedia = qobject_cast<Media *>(sender());
    if (!senderMedia)
        return;
    int mediaRow = _mediaObjects.indexOf(senderMedia);
    emit dataChanged(this->index(mediaRow, 0), this->index(mediaRow, columnCount() - 1));
    emit dataToSaveChanged();
}

QVariant MediaTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Vertical)
        return QAbstractTableModel::headerData(section, orientation, role);
    if (section >= columnCount())
        return QAbstractTableModel::headerData(section, orientation, role);
    Columns column = static_cast<Columns>(section);
    if (role == Qt::DisplayRole)
    {
        switch (column)
        {
        case FilePatch:
            return tr("File Name");
            break;
        case NumberToShow:
            return tr("NTS");
            break;
        case RandomWeigth:
            return tr("RW");
            break;
        default:
            return QAbstractTableModel::headerData(section, orientation, role);
            break;
        }
    }
    else if (role == Qt::ToolTipRole || role == Qt::StatusTipRole)
    {
        switch (column)
        {
        case FilePatch:
            return tr("File Name");
            break;
        case NumberToShow:
            return tr("Number to show");
            break;
        case RandomWeigth:
            return tr("Random weigth");
            break;
        default:
            return QAbstractTableModel::headerData(section, orientation, role);
            break;
        }
    }
    return QAbstractTableModel::headerData(section, orientation, role);
}

void MediaTableModel::moveRow(int row, int newPosition)
{
    if (row >= rowCount() || row < 0)
        return;
    Media * currentMedia = _mediaObjects[row];
    emit beginRemoveRows(QModelIndex(), row, row);
    _mediaObjects.removeAt(row);
    emit endRemoveRows();
    emit beginInsertRows(QModelIndex(), newPosition, newPosition);
    _mediaObjects.insert(newPosition, currentMedia);
    emit endInsertRows();
    emit dataToSaveChanged();
}

Media * MediaTableModel::findNextMedia(const Media *currentMedia, const QList<Media *> ignoredMediaList) const
{
    Media *notConstMedia = const_cast<Media *>(currentMedia);
    if (_mediaObjects.count() == 0)
        return NULL;
    int currentMediaIndex = _mediaObjects.indexOf(notConstMedia);
    Media *result = NULL;
    for (int i = currentMediaIndex + 1; i < _mediaObjects.count(); i++)
    {
        if (_mediaObjects[i]->isValid() && _mediaObjects[i]->isEnabled() && !ignoredMediaList.contains(_mediaObjects[i]))
        {
            result = _mediaObjects[i];
            break;
        }
    }
    if (!result)
    {
        for (int i = 0; i < currentMediaIndex + 1; i++)
        {
            if (_mediaObjects[i]->isValid() && _mediaObjects[i]->isEnabled() && !ignoredMediaList.contains(_mediaObjects[i]))
            {
                result = _mediaObjects[i];
                break;
            }
        }
    }
    return result;
}

Media * MediaTableModel::findNextRandomMedia(const QList<Media *> ignoredMediaList) const
{
    //Составим списко, содержащий индексы медия с учетом веса
    QList<int> randomWeigthIndexList;
    for (int i = 0; i < _mediaObjects.count(); i++)
    {
        if (_mediaObjects[i]->isValid() && _mediaObjects[i]->isEnabled() && !ignoredMediaList.contains(_mediaObjects[i]))
        {
            for (int j = 0; j < _mediaObjects[i]->getRandomWeigth(); j++)
            {
                randomWeigthIndexList << i;
            }
        }
    }
    if (randomWeigthIndexList.count() == 0)
        return NULL;
    //Вернем случайное число
    qsrand(QTime(0,0,0).msecsTo(QTime::currentTime()));
    int randIndex = 0;
    if (randomWeigthIndexList.count() > 1)
        randIndex = qrand()%(randomWeigthIndexList.count() - 1);
    return _mediaObjects[randomWeigthIndexList[randIndex]];
}

Media * MediaTableModel::getMedia(const qint64 id) const
{
    return _mediaObjectsHash[id];
}

void MediaTableModel::setCurrentPlayedMedia(const Media *currentMedia)
{
    _currentPlayedMedia = currentMedia;
    Media * notConstMedia = const_cast<Media *>(currentMedia);
    int currentMediaRow = _mediaObjects.indexOf(notConstMedia);
    emit dataChanged(index(currentMediaRow, 0), index(currentMediaRow, columnCount() - 1));
}
