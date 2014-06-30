
#include "configdialog.h"
#include "ui_configdialog.h"
#include "mdvplayerapplication.h"
#include <QSettings>
#include "settingsdefines.h"

ConfigDialog::ConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigDialog)
{
    ui->setupUi(this);

    QSettings settings;

    ui->languageComboBox->addItem(tr("English"), "en");
    ui->languageComboBox->addItem(tr("Russian"), "ru");

    ui->autosaveProjectCheckBox->setChecked(settings.value(SETTINGS_KEY_AUTOSAVE_ENABLED, DEFAULT_SETTINGS_KEY_AUTOSAVE_ENABLED).toBool());
    ui->autosaveIntervalSpinBox->setValue(settings.value(SETTINGS_KEY_AUTOSAVE_INTERVAL, DEFAULT_SETTINGS_KEY_AUTOSAVE_INTERVAL).toInt());
    int localeIndex = ui->languageComboBox->findData(
                settings.value(SETTINGS_KEY_LANGUAGE, QLocale::system().name()).toString());
    if (localeIndex < 0)
        localeIndex = 0;
    ui->languageComboBox->setCurrentIndex(localeIndex);

    connect(this, SIGNAL(accepted()), this, SLOT(saveParamToSettings()));
}

ConfigDialog::~ConfigDialog()
{
    delete ui;
}

void ConfigDialog::saveParamToSettings()
{
    QSettings settings;
    settings.setValue(SETTINGS_KEY_AUTOSAVE_ENABLED, ui->autosaveProjectCheckBox->isChecked());
    settings.setValue(SETTINGS_KEY_AUTOSAVE_INTERVAL, ui->autosaveIntervalSpinBox->value());
    QString oldTranslation = settings.value(SETTINGS_KEY_LANGUAGE, QLocale::system().name()).toString();
    QString newTranslation = ui->languageComboBox->itemData(ui->languageComboBox->currentIndex()).toString();
    settings.setValue(SETTINGS_KEY_LANGUAGE, newTranslation);
    if (oldTranslation != newTranslation)
        MDVPlayerApplication::reloadTranslations();
}
