#include "extensionsconfigdialog.h"
#include "ui_extensionsconfigdialog.h"
#include "extensions/abstractextentionconfigwidget.h"

ExtensionsConfigDialog::ExtensionsConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExtensionsConfigDialog),
    _engine(NULL),
    _currentExtension(NULL)
{
    ui->setupUi(this);
    connect(ui->extensionsListWidget, SIGNAL(currentRowChanged(int)), this, SLOT(setCurrentExtention(int)));
    connect(ui->extensionEnabledCheckBox, SIGNAL(toggled(bool)), this, SLOT(enableExtention(bool)));
    connect(this, SIGNAL(accepted()), this, SLOT(acceptedSlot()));
}

ExtensionsConfigDialog::~ExtensionsConfigDialog()
{
    delete ui;
}

void ExtensionsConfigDialog::setEngine(ExtensionsEngine *engine)
{
    ui->extensionsListWidget->clear();
    //Удалим старые виджеты
    for (int i = 0; i < ui->extensionConfigStackedWidget->count(); i++)
    {
        QWidget * currentWidget = ui->extensionConfigStackedWidget->widget(i);
        ui->extensionConfigStackedWidget->removeWidget(currentWidget);
    }
    QList<AbstarctExtension *> extensions = engine->getExtensions();
    _extensions = extensions;
    _extentionEnable.clear();
    foreach (AbstarctExtension *extension, extensions)
    {
        QListWidgetItem *newItem = new QListWidgetItem();
        newItem->setText(extension->getName());
        newItem->setIcon(extension->getIcon());
        ui->extensionsListWidget->addItem(newItem);
        AbstractExtentionConfigWidget *configWidget = extension->getWidgetToSettings();
        configWidget->setParent(this);
        configWidget->setParams(extension->getParams());
        ui->extensionConfigStackedWidget->addWidget(configWidget);
        _extentionEnable << extension->getIsEnabled();
    }
    ui->extensionConfigStackedWidget->setCurrentIndex(0);
}

void ExtensionsConfigDialog::setCurrentExtention(const int index)
{
    ui->extensionConfigStackedWidget->setCurrentIndex(index);
    AbstarctExtension *currentExteintion = _extensions[index];
    _currentExtension = currentExteintion;
    if (currentExteintion)
    {
        ui->extansionDescriptionLabel->setText(currentExteintion->getDescription());
        ui->extensionNameLabel->setText(currentExteintion->getName());
        ui->extensionEnabledCheckBox->setChecked(currentExteintion->getIsEnabled());
        ui->extensionConfigStackedWidget->currentWidget()->setEnabled(currentExteintion->getIsEnabled());
    }
}

void ExtensionsConfigDialog::enableExtention(const bool enable)
{
    ui->extensionConfigStackedWidget->currentWidget()->setEnabled(enable);
    int indexOfCurrentExtension = _extensions.indexOf(_currentExtension);
    if (indexOfCurrentExtension != -1 && _extentionEnable.count() == _extensions.count())
        _extentionEnable[indexOfCurrentExtension] = enable;
}

void ExtensionsConfigDialog::acceptedSlot()
{
    if (_extensions.count() != ui->extensionConfigStackedWidget->count())
        return;
    if (_extensions.count() != _extentionEnable.count())
        return;
    for (int i = 0; i < _extensions.count(); i++)
    {
        AbstarctExtension *currentExtention = _extensions[i];
        AbstractExtentionConfigWidget *currentConfigWidget = qobject_cast<AbstractExtentionConfigWidget *>(ui->extensionConfigStackedWidget->widget(i));
        if (!currentConfigWidget)
            continue;
        currentExtention->setIsEnabled(_extentionEnable[i]);
        currentExtention->setParams(currentConfigWidget->getParams());
    }
}
