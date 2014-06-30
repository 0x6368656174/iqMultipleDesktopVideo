#ifndef EXTENSIONSCONFIGDIALOG_H
#define EXTENSIONSCONFIGDIALOG_H

#include <QDialog>
#include "extensionsengine.h"

namespace Ui {
class ExtensionsConfigDialog;
}

class ExtensionsConfigDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ExtensionsConfigDialog(QWidget *parent = 0);
    ~ExtensionsConfigDialog();

    void setEngine(ExtensionsEngine *engine);
    
private slots:
    void setCurrentExtention(const int number);
    void enableExtention(const bool enable);
    void acceptedSlot();

private:
    Ui::ExtensionsConfigDialog *ui;
    ExtensionsEngine *_engine;
    QList<AbstarctExtension *> _extensions;
    AbstarctExtension * _currentExtension;
    QList<bool> _extentionEnable;
};

#endif // EXTENSIONSCONFIGDIALOG_H
