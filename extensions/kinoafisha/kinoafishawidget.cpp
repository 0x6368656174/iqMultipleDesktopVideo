#define SETT_KEY_KINOAFISHA_QML_PATCH "kinoafishaQmlDirPatch"

#define DEFATUL_SETT_VALUE_KINOAFISHA_QML_PATCH "/extensions/kinoafisha/qml/"

#include "kinoafishawidget.h"
#include <QSettings>
#include <QDir>
#include <QGraphicsObject>
#include <QDeclarativeEngine>
#include <QDeclarativeItem>
#include <QDebug>
#include <QCoreApplication>


KinoafishaWidget::KinoafishaWidget(QWidget *parent) :
    QDeclarativeView(parent)
{
    setResizeMode(QDeclarativeView::SizeRootObjectToView);
}

void KinoafishaWidget::setCinamaList(const QList<Cinema *> cinemaList)
{
    QTime progressTime;
    qDebug() << "Set cinema list to qmlView...";
    progressTime.start();
    QSettings settings;
    QString affishaQmlPatch = settings.value(SETT_KEY_KINOAFISHA_QML_PATCH,
                                             QCoreApplication::applicationDirPath() +
                                             DEFATUL_SETT_VALUE_KINOAFISHA_QML_PATCH).toString();
    qDebug() << "Load qml " << affishaQmlPatch + "/Affisha.qml...";
    QTime loadTime;
    loadTime.start();
    if (!QFile::exists(affishaQmlPatch + "/Affisha.qml"))
    {
        qWarning() << "Affisha.qml not found";
        return;
    }
    setSource(QUrl::fromLocalFile(affishaQmlPatch + "/Affisha.qml"));
    qDebug() << "Load qml finished in " << loadTime.elapsed();
    QDeclarativeItem * rootItem = qobject_cast<QDeclarativeItem *>(rootObject());
    qDebug() << "Kinoafisha widget width " << _screenWidth <<" height " << _screenHeight;
    rootItem->setWidth(_screenWidth);
    rootItem->setHeight(_screenHeight);
    connect(rootObject(), SIGNAL(trailerPlayFinished(int)), this, SIGNAL(traylerPlayedFinised(int)));
    foreach (Cinema *currentCinema, cinemaList)
    {
        addCinema(currentCinema);
    }
    qDebug() << "Setting cinema list to qmlView finished in " << progressTime.elapsed() << " msec";
}

void KinoafishaWidget::addCinema( Cinema *cinema)
{
    if (!cinema)
        return;
    qDebug() << "Add cinema to film billboard: name " << cinema->getName() << ", startDateTime " << cinema->getStartDateTime().toString();
    connect(cinema, SIGNAL(freePlaceChanged(int)), this, SLOT(cinemaFreePlaceChanged(int)));
    QVariantList price;
    foreach (int var, cinema->getPrice())
    {
        price << var;
    }
    QObject * cinemaObject = qobject_cast<QObject *>(cinema);
    QMetaObject::invokeMethod(rootObject(), "addCinemaSeance",
                              Q_ARG(QVariant, QVariant::fromValue(cinemaObject)));
}

void KinoafishaWidget::setCinemaFreePlace(const QString id, const int freePlace)
{
    QMetaObject::invokeMethod(rootObject(), "setCinemaFreePlace",
                              Q_ARG(QVariant, id),
                              Q_ARG(QVariant, freePlace));
}

void KinoafishaWidget::playTraylers()
{
    QMetaObject::invokeMethod(rootObject(), "playAllTraylers");
}

void KinoafishaWidget::pauseTraylers()
{
    QMetaObject::invokeMethod(rootObject(), "pauseAllTraylers");
}

void KinoafishaWidget::playTrayler(Media *media)
{
    QObject *mediaObject = qobject_cast<QObject *>(media);
    if (!mediaObject)
        return;
    QMetaObject::invokeMethod(rootObject(), "playTrayled",
                              Q_ARG(QVariant, QVariant::fromValue(mediaObject)));
}

QList<qint64> KinoafishaWidget::getPlayedMediaId() const
{
    QList<qint64> result;
    QVariant qmlReturValue;
    QMetaObject::invokeMethod(rootObject(), "playedTraylerIdList",
                              Q_RETURN_ARG(QVariant, qmlReturValue));
    if (qmlReturValue.type() != QVariant::List)
        return result;
    QList<QVariant> qmlReturnList = qmlReturValue.toList();
    foreach (QVariant id, qmlReturnList)
    {
        if (id.canConvert(QVariant::LongLong))
            result << id.toLongLong();
    }
    return result;
}

void KinoafishaWidget::cinemaFreePlaceChanged(const int freePlace)
{
    Cinema *senderCinema = qobject_cast<Cinema *>(sender());
    if (senderCinema)
    {
        setCinemaFreePlace(senderCinema->getId(), freePlace);
    }
}
