#ifndef CINEMA_H
#define CINEMA_H

#include <QObject>
#include <QDateTime>
#include <QAtomicInt>

class Cinema : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString cinemaId READ getId WRITE setId)
    Q_PROPERTY(QString name READ getName WRITE setName)
    Q_PROPERTY(QString hallName READ getHallName WRITE setHallName)
    Q_PROPERTY(QString ageLimit READ getAgeLimit WRITE setAgeLimit)
    Q_PROPERTY(bool is3D READ getIs3D WRITE setIs3D)
    Q_PROPERTY(int freePlace READ getFreePlace WRITE setFreePlace)
    Q_PROPERTY(QDateTime startDateTime READ getStartDateTime WRITE setStartDateTime)
    Q_PROPERTY(int duration READ getDurationMin WRITE setDurationMin)
    Q_PROPERTY(QList<int> price READ getPrice WRITE setPrice)

public:
    explicit Cinema(QObject *parent = 0);

    ~Cinema();

    inline QString getId() const {return _id;}

    inline void setId(const QString &id) {_id = id;}
    
    inline QString getName() const {return _name;}

    inline QString getHallName() const {return _hallName;}

    inline QString getAgeLimit() const {return _ageLimit;}

    inline bool getIs3D() const {return _is3D;}

    inline QList<int> getPrice() const {return _price;}

    inline int getFreePlace() const {return _freePlace;}

    inline QDateTime getStartDateTime() const {return _startDateTime;}

    inline int getDurationMin() const {return _durationMin;}

    inline void setName(const QString & name) {_name = name;}

    inline void setHallName(const QString & hallName) {_hallName = hallName;}

    inline void setAgeLimit(const QString &ageLimit) {_ageLimit = ageLimit;}

    inline void setIs3D(const bool is3D) {_is3D = is3D;}

    inline void setPrice(const QList<int> price) {_price = price;}

    void setFreePlace(const int freePlace);

    inline void setStartDateTime(const QDateTime & startDateTime) {_startDateTime = startDateTime;}

    inline void setDurationMin(const int duration) {_durationMin = duration;}

signals:
    void freePlaceChanged(const int newFreePlace);

private:
    QString _id;
    QString _name;
    QString _hallName;
    QString _ageLimit;
    bool _is3D;
    QList<int> _price;
    int _freePlace;
    QDateTime _startDateTime;
    int _durationMin;

    static QAtomicInt _instanceCount;
};

#endif // CINEMA_H
