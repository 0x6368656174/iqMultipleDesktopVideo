#ifndef KINOAFISHAWIDGET_H
#define KINOAFISHAWIDGET_H

#include <QDeclarativeView>
#include "cinema.h"
#include "media.h"

class KinoafishaWidget : public QDeclarativeView
{
    Q_OBJECT
public:
    explicit KinoafishaWidget(QWidget *parent = 0);
    
//    Cinema * getCinema(const QString &hallName, const QDateTime &startDateTime) const;

    void setCinamaList(const QList<Cinema *> cinemaList);

    void playTrayler(Media *media);

    QList<qint64> getPlayedMediaId() const;

    inline void setScreenSize(const int width, const int height) {_screenWidth = width; _screenHeight = height;}

signals:
    void traylerPlayedFinised(const int traylerId);

public slots:
    void playTraylers();
    void pauseTraylers();

private slots:
    void cinemaFreePlaceChanged(const int freePlace);

private:
//    QHash<QString, QHash<qint64, Cinema *> > _kinoafisha;

    int _screenWidth;
    int _screenHeight;
    void addCinemaToQML(const Cinema * cinema);
    void setCinemaFreePlace(const QString id, const int freePlace);
    void addCinema(Cinema *cinema);
};

#endif // KINOAFISHAWIDGET_H
