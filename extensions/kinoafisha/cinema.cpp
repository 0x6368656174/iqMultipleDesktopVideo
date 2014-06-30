#include "cinema.h"
#include <QDebug>

QAtomicInt Cinema::_instanceCount = 0;
Cinema::Cinema(QObject *parent) :
    QObject(parent),
    _id(""),
    _name(""),
    _hallName(""),
    _ageLimit(""),
    _is3D(false),
    _price(QList<int>()),
    _freePlace(-1),
    _startDateTime(QDateTime::currentDateTime()),
    _durationMin(0)
{
    qDebug() << "Creating new cinema object. Total ciname object count " << _instanceCount.fetchAndAddRelease(1) + 1;
}

Cinema::~Cinema()
{
    qDebug() << "Destroyed cinema object. Total cinema object " << _instanceCount.fetchAndAddRelease(-1) - 1;
}

void Cinema::setFreePlace(const int freePlace)
{
    if (_freePlace != freePlace)
    {
        _freePlace = freePlace;
        emit freePlaceChanged(freePlace);
    }
}
