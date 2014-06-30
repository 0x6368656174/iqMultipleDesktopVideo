#define NAME "kinoafisha"
#define DB_IDENT "kinoafishaDBConect"

#include "extensions/extensionsqlconfigwidgetdefines.h"
#include "kinoafishadefines.h"
#include "kinoafishaextension.h"
#include <QHash>
#include <QVariant>
#include <QIcon>
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QtConcurrentRun>
#include <exception>
#include "qmlvideo.h"

KinoafishaExtension::KinoafishaExtension(QObject *parent) :
    AbstarctExtension(parent),
    _freePlaceUpdateTimer(new QTimer(this)),
    _updateTimer(new QTimer(this)),
    _cinemaListFutureWatcher(new QFutureWatcher<QList<Cinema *> >(this)),
    _updateFreePlaceWatcher(new QFutureWatcher<void> (this)),
    _dayOfLastUpdate(-1),
    _screen(NULL)
{
    connect(_cinemaListFutureWatcher, SIGNAL(finished()), this, SLOT(onCinemaListReceived()));

    setScreen(new Screen());

    //Зарегистрируем нужные типы QML
    qmlRegisterType<QMLVideo>("KinoafishaExtension", 1, 0, "QMLVideo");

    connect(_freePlaceUpdateTimer, SIGNAL(timeout()), this, SLOT(updateCinemaFreePlace()));
    connect(_updateTimer, SIGNAL(timeout()), this, SLOT(updateIfNeed()));
    _freePlaceUpdateTimer->setInterval(30000);
    _freePlaceUpdateTimer->start();
    _updateTimer->setInterval(2000);
    _updateTimer->start();
}

KinoafishaExtension::~KinoafishaExtension()
{
    QHashIterator<QString, KinoafishaWidget *> i (_widgetsToShow);
    while (i.hasNext())
    {
        i.next();
        if (i.value())
            i.value()->deleteLater();
    }
}

QString KinoafishaExtension::getIdentificationName() const
{
    return NAME;
}

AbstractExtentionConfigWidget * KinoafishaExtension::getWidgetToSettings() const
{
    return new ConfigWidget();
}

QString KinoafishaExtension::getName() const
{
    return tr("Film Billboard");
}

QString KinoafishaExtension::getDescription() const
{
    return tr("Displays beautiful film billboard, compiled in a database.");
}

QHash<QString, QVariant> KinoafishaExtension::getParams() const
{
    return _params;
}

void KinoafishaExtension::setParams(const QHash<QString, QVariant> params)
{
    if (params == _params)
        return;
    _params = params;
    _freePlaceUpdateTimer->setInterval(params[PARAM_FREE_PLACE_UPDATE_INTERVAL].toInt() * 1000);
    _dayOfLastUpdate = -1;
    reloadKinoafishaWidget();
    emit dataChanged();
}

void KinoafishaExtension::updateIfNeed()
{
    if (!_params.contains(PARAM_RELOAD_KINOAFISHA_TIME))
        return;
    //Если сегодня уже обновляли, то ничо не делаем
    if (QDateTime::currentDateTime().date().day() == _dayOfLastUpdate)
        return;
    //Если сейчас еще время меньшее, чем то, в которое нужно обновлять, то ничего делать не будем
    if (QTime::currentTime() < _params[PARAM_RELOAD_KINOAFISHA_TIME].toTime())
        return;
    //Попытаемся вызвать обновление виджета
    reloadKinoafishaWidget();
}

void KinoafishaExtension::play()
{
    foreach (KinoafishaWidget * w, _widgetsToShow)
    {
        w->playTraylers();
    }
}

void KinoafishaExtension::pause()
{
    foreach (KinoafishaWidget *w, _widgetsToShow)
    {
        w->pauseTraylers();
    }
}

QGraphicsScene * KinoafishaExtension::getSceneToShow(const int width, const int height) const
{
    QString resolutionString = QString("%0x%1").arg(width).arg(height);
    if (!_widgetsToShow[resolutionString])
    {
        qDebug() << "Created new kinoafisha extension view " << width << "x" << height;
        KinoafishaWidget * newKinoafisha = createKinoafisha(width, height);
        _widgetsToShow[resolutionString] = newKinoafisha;
        _widgetsToShow[resolutionString]->setCinamaList(_cinemas);
        connect(newKinoafisha, SIGNAL(traylerPlayedFinised(int)), this, SLOT(onTrailerPlayedFinished(int)));
    }
    return _widgetsToShow[resolutionString]->scene();
}

void KinoafishaExtension::onTrailerPlayedFinished(const int id)
{
    KinoafishaWidget *senderWidget = qobject_cast<KinoafishaWidget *>(sender());
    if (!senderWidget)
        return;
    //Найдем медия по его ид и уменьшим кол-во просмотров
    if (!_screen)
        return;
    Media *oldMedia = _screen->getMedia(id);
    if (oldMedia)
    {
        if (oldMedia->getNumberToShow() > 0)
        {
            oldMedia->setNumberToShow(oldMedia->getNumberToShow() - 1);
        }
    }
    //Получим спиок уже воспроизводимых медиа
    QList<qint64> playedMediaIds = senderWidget->getPlayedMediaId();
    //Созададим список медиа, которые мы не будем учитывать в поске
    QList<Media *> ignoredMediaList;
    foreach (qint64 id, playedMediaIds)
    {
        Media *media = _screen->getMedia(id);
        if (media)
            ignoredMediaList << media;
    }
    //Получим следующее медия для показа с учетом игнорируемых медиа
    Media *nextMedia = _screen->getNextMediaToPlay(ignoredMediaList);
    //Если такое медиа получить не удалось, то просто получим медиа без учета игнориуемых
    if (!nextMedia)
        nextMedia = _screen->getNextMediaToPlay();
    //Не удалось, значит ничего показывать не надо
    if (!nextMedia)
        return;
    //Отправим следующее медиа для показа
    senderWidget->playTrayler(nextMedia);
}

KinoafishaWidget * KinoafishaExtension::createKinoafisha(const int width, const int height) const
{
    KinoafishaWidget * result = new KinoafishaWidget();
    qDebug() << "Kinoafisha screen resized to width " << width << " height " << height;
    result->setScreenSize(width, height);
    return result;
}

QIcon KinoafishaExtension::getIcon() const
{
    return QIcon(":/icons/popcorn.png");
}

bool cinemaLassThen(Cinema * c1, Cinema * c2)
{
    return c1->getStartDateTime() < c2->getStartDateTime();
}

bool KinoafishaExtension::reloadKinoafishaWidget()
{
    if (_cinemaListFutureWatcher->isRunning())
        return false;
    QFuture<QList<Cinema *> > future = QtConcurrent::run(this, &KinoafishaExtension::getCinameList, this->thread());
    _cinemaListFutureWatcher->setFuture(future);
    //Установим день последнего обновления в сегодня
    _dayOfLastUpdate = QDateTime::currentDateTime().date().day();
    return true;
}

void KinoafishaExtension::updateCinemaFreePlace()
{
    if (_updateFreePlaceWatcher->isRunning())
        return;
    QFuture<void> future = QtConcurrent::run(this, &KinoafishaExtension::setCinemaFreePlace, _cinemas);
    _updateFreePlaceWatcher->setFuture(future);
}

void KinoafishaExtension::onCinemaListReceived()
{
    QList<Cinema *> cinemaList = _cinemaListFutureWatcher->future().result();
    setCinemaListToWidgets(cinemaList);
}

void KinoafishaExtension::setCinemaListToWidgets(const QList<Cinema *> cinemaList)
{
    //Удалим все старые киносеансы
    foreach (Cinema *cinema, _cinemas)
    {
        cinema->deleteLater();
    }
    _cinemas.clear();

    _cinemas = cinemaList;

    foreach(Cinema *cinema, _cinemas)
    {
        cinema->setParent(this);
    }

    //Отсортируем наши сеансы в порядке от самого раннего к самому позднему
    qSort(_cinemas.begin(), _cinemas.end(), cinemaLassThen);

    QHashIterator<QString, KinoafishaWidget *> i (_widgetsToShow);
    while(i.hasNext())
    {
        i.next();
        i.value()->setCinamaList(_cinemas);
    }
}

QList<Cinema *> KinoafishaExtension::getCinameList(QThread * objectThread)
{
    QTime progressTime;
    qDebug() << "Getting cinema list...";
    progressTime.start();
    QList<Cinema *> result;
    if (_params.isEmpty())
        return result;
    if (_params[PARAM_DATABASE_DRIVER].toString().isEmpty())
        return result;
    for (int i = 0; i < 1; i++)
    {
        QSqlDatabase db = createDBConnection(__FUNCTION__ );
        if (!db.isValid())
        {
            qWarning("Database connection is invalid");
            break;
        }
        if (_params[PARAM_CINEMA_LIST_QUERY].toString().isEmpty())
        {
            break;

        }
        QSqlQuery query (db);
        if (!query.exec(_params[PARAM_CINEMA_LIST_QUERY].toString()))
        {
            qWarning() << "Error on cinema list query: " << query.lastError().text();
            break;
        }
        int idNo = query.record().indexOf("ID");
        if (idNo < 0)
        {
            qWarning() << "Can not find required field ID";
            break;

        }
        int nameNo = query.record().indexOf("NAME");
        if (nameNo < 0)
        {
            qWarning() << "Can not find required field NAME";
            break;

        }
        int hallNameNo = query.record().indexOf("HALLNAME");
        if (hallNameNo < 0)
        {
            qWarning() << "Can not find required field HALLNAME";
            break;

        }
        int startDateTimeNo = query.record().indexOf("STARTDATETIME");
        if (startDateTimeNo < 0)
        {
            qWarning() << "Can not find required field STARTDATETIME";
            break;

        }
        int durationNo = query.record().indexOf("DURATION");
        int freeplaceNo = query.record().indexOf("FREEPLACE");
        int is3DNo = query.record().indexOf("IS3D");
        int ageLimit = query.record().indexOf("AGELIMIT");

        //Подготовим запрос для получения списка цен
        QSqlQuery priceQuery (db);
        bool priceAvailible = false;
        if (!_params[PARAM_CINEMA_PRICE_QUERY].toString().isEmpty())
        {
            priceAvailible = priceQuery.prepare(_params[PARAM_CINEMA_PRICE_QUERY].toString());
            if (!priceAvailible)
            {
                qDebug() << "Price SQL Error: " << priceQuery.lastError().text();
            }
        } else
        {
            qDebug() << "Price not availible.";
        }

        //Пройдемся по всем новым киносеансам
        while (query.next())
        {
            Cinema *newCinema = new Cinema();
            newCinema->setId(query.value(idNo).toString());
            newCinema->setName(query.value(nameNo).toString());
            newCinema->setHallName(query.value(hallNameNo).toString());
            newCinema->setStartDateTime(query.value(startDateTimeNo).toDateTime());
            if (durationNo > 0)
                newCinema->setDurationMin(query.value(durationNo).toInt());
            if (freeplaceNo > 0)
                newCinema->setFreePlace(query.value(freeplaceNo).toInt());
            if (is3DNo > 0)
                newCinema->setIs3D(query.value(is3DNo).toBool());
            if (ageLimit > 0)
                newCinema->setAgeLimit(query.value(ageLimit).toString());
            //Добавим цены
            if (priceAvailible)
            {
                priceQuery.bindValue(":cinemaId", newCinema->getId());
                if (priceQuery.exec())
                {
                    QList <int> prices;
                    int priceNo = priceQuery.record().indexOf("PRICE");
                    while (priceQuery.next())
                    {
                        if (priceQuery.value(priceNo).toInt() > 0)
                            prices << priceQuery.value(priceNo).toInt();
                    }
                    newCinema->setPrice(prices);
                }
            }
            if (objectThread)
                newCinema->moveToThread(objectThread);
            result << newCinema;
        }
    }
    QSqlDatabase::removeDatabase(__FUNCTION__ );
    qDebug() << "Getting cinema list finished in " << progressTime.elapsed() << " msec";
    return result;
}

QSqlDatabase KinoafishaExtension::createDBConnection(const QString &name)
{
    QTime progressTime;
    qDebug() << "Creating DB connection...";
    progressTime.start();
    for (int i = 0; i < 1; i++)
    {
        QSqlDatabase db = QSqlDatabase::addDatabase(_params[PARAM_DATABASE_DRIVER].toString(), name);
        if (_params[PARAM_DATABASE_DRIVER].toString() != "QSQLITE" && _params[PARAM_DATABASE_DRIVER].toString() != "QSQLITE2")
        {
            if (_params[PARAM_DATABASE_NAME].toString().isEmpty())
            {
                qWarning() << "Not set database name";
                break;
            }
            if (!_params[PARAM_DATABASE_HOST_NAME].toString().isEmpty())
                db.setHostName(_params[PARAM_DATABASE_HOST_NAME].toString());
            if (_params[PARAM_DATABASE_PORT].toInt() > 0)
                db.setPort(_params[PARAM_DATABASE_PORT].toInt());
            if (!_params[PARAM_DATABASE_NAME].toString().isEmpty())
                db.setDatabaseName(_params[PARAM_DATABASE_NAME].toString());
            if (!_params[PARAM_DATABASE_USER].toString().isEmpty())
                db.setUserName(_params[PARAM_DATABASE_USER].toString());
            if (!_params[PARAM_DATABASE_PASSWORD].toString().isEmpty())
                db.setPassword(_params[PARAM_DATABASE_PASSWORD].toString());
        } else
        {
            if (!_params[PARAM_DATABASE_PATCH].toString().isEmpty())
                db.setDatabaseName(_params[PARAM_DATABASE_PATCH].toString());
        }
        bool ok = db.open();
        if (!ok)
        {
            qWarning("Can not connect to database: DRIVER: %s, HOSTNAME: %s, PORT: %d, DBNAME: %s, DBUSER: %s, DBPASS: %s, DBPATCH: %s",
                     qPrintable(_params[PARAM_DATABASE_DRIVER].toString()),
                     qPrintable(_params[PARAM_DATABASE_HOST_NAME].toString()),
                     _params[PARAM_DATABASE_PORT].toInt(),
                     qPrintable(_params[PARAM_DATABASE_NAME].toString()),
                     qPrintable(_params[PARAM_DATABASE_USER].toString()),
                     qPrintable(_params[PARAM_DATABASE_PASSWORD].toString()),
                     qPrintable(_params[PARAM_DATABASE_PATCH].toString()));
            qWarning() << "SQL Error: " << db.lastError().text();
            break;
        }
        qDebug() << "Creating DB connection finished in " << progressTime.elapsed() << " msec";
        return db;
    }
    QSqlDatabase::removeDatabase(name);
    qDebug() << "Creating DB connection finished in " << progressTime.elapsed() << " msec";
    return QSqlDatabase();
}

void KinoafishaExtension::setCinemaFreePlace(QList<Cinema *> cinemaList)
{
    QTime progressTime;
    qDebug() << "Getting cinema free place list...";
    progressTime.start();
    if (_params.isEmpty())
        return;
    for (int i = 0; i < 1; i++)
    {
        QSqlDatabase db = createDBConnection(__FUNCTION__ );
        if (!db.isValid() || !db.isOpen())
        {
            qWarning("Database connection is invalid");
            break;
        }
        QSqlQuery query (db);
        if (!query.exec(_params[PARAM_CINEMA_LIST_QUERY].toString()))
        {
            qWarning() << "Error on cinema list query: " << query.lastError().text();
            break;
        }
        int idNo = query.record().indexOf("ID");
        if (idNo < 0)
        {
            qWarning() << "Can not find required field ID";
            break;
        }
        int freePlaceNo = query.record().indexOf("FREEPLACE");
        if (freePlaceNo < 0)
        {
            break;
        }

        QHash<QString, Cinema *> cinemaHash;
        foreach (Cinema * cinema, cinemaList)
        {
            cinemaHash[cinema->getId()] = cinema;
        }
        while (query.next())
        {
            QString cinemaId = query.value(idNo).toString();
            Cinema *cinema = cinemaHash[cinemaId];
            if (cinema)
                cinema->setFreePlace(query.value(freePlaceNo).toInt());
        }
    }
    QSqlDatabase::removeDatabase(__FUNCTION__ );
    qDebug() << "Getting cinema free place list finished in " << progressTime.elapsed() << " msec";
}

Screen * KinoafishaExtension::getScreen() const
{
    return _screen;
}

void KinoafishaExtension::setScreen(Screen *screen)
{
    if (_screen && _screen->parent() == this)
    {
        _screen->deleteLater();
    }
    _screen = screen;
    _screen->setParent(this);
    _screen->setIcont(getIcon());
    _screen->setName(getName());
    _screen->setIsRealScreen(false);
    _screen->setDeleteRenameEnabled(false);
    connect(_screen, SIGNAL(dataToSaveChanged()), this, SIGNAL(dataChanged()));
}
