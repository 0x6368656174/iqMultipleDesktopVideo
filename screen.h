#ifndef SCREEN_H
#define SCREEN_H

#include <QObject>
#include <QIcon>
#include <QPointer>
#include <QAtomicInt>
#include "mediatablemodel.h"

class Screen : public QObject
{
    Q_OBJECT
public:
    explicit Screen(QObject *parent = 0);

    ~Screen();

    void setMediaModel(MediaTableModel *model);

    inline MediaTableModel *getMediaModel() const {return _mediaModel;}

    Media * getNextMediaToPlay(const QList<Media *> ignoredMediaList = QList<Media *>()) const;

    Media * getMedia(const qint64 id) const;

    void setPlayedMedia(const Media * media);

    inline QString getName() const {return _name;}

    inline bool getRandomEnabled() const {return _randomEnabled;}

    inline int getNumberOfScreen() const {return _numberOfScreen;}

    void decreaseCurrentMediaNumberToShow();

    inline bool getIsRealScreen() const {return _isRealScreen;}

    inline void setIsRealScreen(const bool isReal) {_isRealScreen = isReal;}

    inline bool getDeleteRenameEnabled() const {return _deleteRenameEnabled;}

    inline void setDeleteRenameEnabled(const bool enabled) {_deleteRenameEnabled = enabled;}

    inline QIcon getIcon() const {return _icon;}

    inline void setIcont(const QIcon &icon) {_icon = icon;}

public slots:
    void setName(const QString &name);

    void setRandomEnabled(const bool enabled);

    void setNumberOfScreen(const int number);

signals:
    void mediaModelChanged(MediaTableModel *newModel);

    void nameChanged(const QString &newName);

    void randomEnabledChanged(const bool newRandomEnabled);

    void numberOfScreenChanged(const int newNumberOfScreen);

    void dataToSaveChanged();
    
private:
    MediaTableModel *_mediaModel;
    QString _name;
    bool _randomEnabled;
    int _numberOfScreen;
    mutable QPointer<Media> _currentMediaToPlay;
    bool _isRealScreen;
    bool _deleteRenameEnabled;
    QIcon _icon;

    static QAtomicInt _instanceCount;
};

#endif // SCREEN_H
