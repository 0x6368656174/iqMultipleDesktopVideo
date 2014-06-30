#ifndef KINOAFISHAEXTENSION_H
#define KINOAFISHAEXTENSION_H

#include "../abstarctextension.h"
#include "kinoafishawidget.h"
#include "configwidget.h"
#include <QTimer>
#include <QSqlDatabase>
#include <QFutureWatcher>
#include <QPointer>
#include <QGraphicsScene>
#include "screen.h"

class KinoafishaExtension : public AbstarctExtension
{
    Q_OBJECT
public:
    explicit KinoafishaExtension(QObject *parent = 0);

    ~KinoafishaExtension();
    
    QString getIdentificationName() const;

    QString getName() const;

    QString getDescription() const;

    QGraphicsScene * getSceneToShow(const int width, const int height) const;

    AbstractExtentionConfigWidget * getWidgetToSettings() const;

    QHash<QString, QVariant> getParams() const;

    void setParams(const QHash<QString, QVariant> params);

    QIcon getIcon() const;

    Screen * getScreen() const;

    void setScreen(Screen *screen);

    void play();

    void pause();

private slots:
    void updateIfNeed();
    bool reloadKinoafishaWidget();
    void updateCinemaFreePlace();
    void setCinemaListToWidgets(const QList<Cinema *> cinemaList);
    void onCinemaListReceived();
    void onTrailerPlayedFinished(const int id = -1);

private:
    mutable QHash<QString, KinoafishaWidget *> _widgetsToShow;

    QHash<QString, QVariant> _params;

    QList<Cinema *> _cinemas;

    QTimer *_freePlaceUpdateTimer;
    QTimer *_updateTimer;

    QFutureWatcher<QList<Cinema *> > *_cinemaListFutureWatcher;
    QFutureWatcher<void> *_updateFreePlaceWatcher;

    int _dayOfLastUpdate;
    QPointer<Screen> _screen;

    KinoafishaWidget * createKinoafisha(const int width, const int height) const;

    QSqlDatabase createDBConnection(const QString &name);
    QList<Cinema *> getCinameList(QThread * objectThread = NULL);
    void setCinemaFreePlace(QList<Cinema *> cinemaList);

};

#endif // KINOAFISHAEXTENSION_H
