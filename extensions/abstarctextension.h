#ifndef ABSTARCTEXTENSION_H
#define ABSTARCTEXTENSION_H

#include <QObject>
#include <QHash>
#include <QGraphicsScene>
#include "abstractextentionconfigwidget.h"
#include "screen.h"

class AbstarctExtension : public QObject
{
    Q_OBJECT
public:
    explicit AbstarctExtension(QObject *parent = 0);

    virtual QString getIdentificationName() const = 0;

    virtual QString getName() const = 0;

    virtual QString getDescription() const {return "";}
    
    virtual QGraphicsScene * getSceneToShow(const int width, const int height) const = 0;

    virtual AbstractExtentionConfigWidget * getWidgetToSettings() const = 0;

    virtual QHash<QString, QVariant> getParams() const = 0;

    virtual void setParams(const QHash<QString, QVariant> params) = 0;

    virtual QIcon getIcon() const = 0;

    virtual Screen * getScreen() const {return NULL;}

    inline bool getIsEnabled() const {return _enabled;}

    void setIsEnabled(const bool enabled);

public slots:
    virtual void setScreen(Screen * screen) {screen->deleteLater();}

    virtual void play() = 0;

    virtual void pause() = 0;

signals:
    void dataChanged() const;

private:
    bool _enabled;
};

#endif // ABSTARCTEXTENSION_H
