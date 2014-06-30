#ifndef ABSTRACTEXTENTIONCONFIGWIDGET_H
#define ABSTRACTEXTENTIONCONFIGWIDGET_H

#include <QWidget>

class AbstractExtentionConfigWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AbstractExtentionConfigWidget(QWidget *parent = 0);
    
    virtual QHash<QString, QVariant> getParams() const = 0;

    virtual void setParams(const QHash<QString, QVariant> params) = 0;
};

#endif // ABSTRACTEXTENTIONCONFIGWIDGET_H
