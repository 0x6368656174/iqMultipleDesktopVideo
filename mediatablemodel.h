#ifndef MEDIATABLEMODEL_H
#define MEDIATABLEMODEL_H

#include <QAbstractTableModel>
#include <QAtomicInt>
#include "media.h"

class MediaTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit MediaTableModel(QObject *parent = 0);

    ~MediaTableModel();
    
    enum MyRole
    {
        MediaPnt = Qt::UserRole + 1,
        ExtensionName,
        ExtensionIdentificationName
    };

    enum Columns
    {
        FilePatch,
        NumberToShow,
        RandomWeigth,
        ENDColumn
    };
    
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    inline int rowCount(const QModelIndex &parent = QModelIndex()) const {Q_UNUSED(parent); return _mediaObjects.count();}

    inline int columnCount(const QModelIndex &parent = QModelIndex()) const {Q_UNUSED(parent); return ENDColumn;}

    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());

    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole );

    void moveRow(int row, int newPosition);

    Media * findNextMedia(const Media * currentMedia, const QList<Media *> ignoredMediaList = QList<Media *>()) const;

    Media * findNextRandomMedia(const QList<Media *> ignoredMediaList = QList<Media *>()) const;

    Media * getMedia(const qint64 id) const;

    void setCurrentPlayedMedia(const Media * currentMedia);

signals:
    void dataToSaveChanged();

private slots:
    void childDataChanded();

private:
    QList<Media *> _mediaObjects;
    QHash<qint64, Media *> _mediaObjectsHash;
    const Media * _currentPlayedMedia;

    static QAtomicInt _instanceCount;
};

#endif // MEDIATABLEMODEL_H
