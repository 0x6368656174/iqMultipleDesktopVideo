#ifndef CONFIGWIDGET_H
#define CONFIGWIDGET_H

#include "../abstractextentionconfigwidget.h"

namespace Ui {
class ConfigWidget;
}

class ConfigWidget : public AbstractExtentionConfigWidget
{
    Q_OBJECT
    
public:
    explicit ConfigWidget(QWidget *parent = 0);
    ~ConfigWidget();

    QHash<QString, QVariant> getParams() const;

    void setParams(const QHash<QString, QVariant> params);

private slots:
    void showHelp();
    
private:
    Ui::ConfigWidget *ui;
};

#endif // CONFIGWIDGET_H
