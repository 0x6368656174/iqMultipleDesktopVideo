#ifndef MEDIA_H
#define MEDIA_H

#include <QObject>
#include <QVariant>
#include <QTime>
#include <QDate>

class Media : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString filePatch READ getFilePatch WRITE setFilePatch NOTIFY filePatchChanged)
    Q_PROPERTY(QString title READ getTitle WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(qint64 id READ getId)
    Q_PROPERTY(qint64 startPosition READ getStartPositionMSecs WRITE setStartPositionMSecs)
    Q_PROPERTY(qint64 endPosition READ getEndPositionMSecs WRITE setEndPositionMSecs)
    Q_PROPERTY(qreal birghtness READ getBrightness WRITE setBrightness)
    Q_PROPERTY(qreal contrast READ getContrast WRITE setContrast)
    Q_PROPERTY(qreal hue READ getHue WRITE setHue)
    Q_PROPERTY(qreal saturation READ getSaturation WRITE setSaturation)

public:
    explicit Media(QObject *parent = 0);
    
    inline QString getFilePatch() const {return _filePatch;}

    inline int getNumberToShow() const {return _numberToShow;}

    inline int getRandomWeigth() const {return _randomWeigth;}

    inline qint64 getStartPositionMSecs() const {return _startPositionMSecs;}

    inline qint64 getEndPositionMSecs() const {return _endPositionMSecs;}

    inline qreal getBrightness() const {return _brightness;}

    inline qreal getContrast() const {return _contrast;}

    inline qreal getHue() const {return _hue;}

    inline qreal getSaturation() const {return _saturation;}

    void setFilePatch(const QString & patch);

    void setNumberToShow(const int numberToShow);

    void setRandomWeigth(const int weigth);

    void setStartPositionMSecs(const qint64 msecs);

    void setEndPositionMSecs(const qint64 msecs);

    void setBrightness(const qreal brightness);

    void setContrast(const qreal contrast);

    void setHue(const qreal hue);

    void setSaturation(const qreal saturation);

    inline bool getIsExtention() const {return !_extentionIdentificationName.isEmpty();}

    inline QString getExtentionIdentificationName() const {return _extentionIdentificationName;}

    inline void setExtentionIdentificationName(const QString name) {_extentionIdentificationName = name;}

    inline QTime getStartTime() const {return _startTime;}

    inline QTime getEndTime() const {return _endTime;}

    inline QDate getStartDate() const {return _startDate;}

    inline QDate getEndDate() const {return _endDate;}

    void setStartTime(const QTime &time);

    void setEndTime(const QTime &time);

    void setStartDate(const QDate &date);

    void setEndDate(const QDate &date);

    QHash<QString, QVariant> getParams() const;

    void setParams(QHash<QString, QVariant> params);

    bool isValid() const;

    bool isEnabled() const;

    inline qint64 getId() const {return _id;}

    void setTitle(const QString &title);

    inline QString getTitle() const {return _title;}

signals:
    void dataChanged();
    void filePatchChanged();
    void titleChanged();

private:
    qint64 _id;
    QString _filePatch;
    QString _title;
    int _numberToShow;
    int _randomWeigth;
    qint64 _startPositionMSecs;
    qint64 _endPositionMSecs;
    qreal _brightness;
    qreal _contrast;
    qreal _hue;
    qreal _saturation;
    QString _extentionIdentificationName;
    QTime _startTime;
    QTime _endTime;
    QDate _startDate;
    QDate _endDate;
    static qint64 _lastId;
};

Q_DECLARE_METATYPE(Media *)

#endif // MEDIA_H
