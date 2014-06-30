#include "extensionsqlconfigwidgetdefines.h"
#include "extensionsqlconfigwidget.h"
#include "ui_extensionsqlconfigwidget.h"
#include <QSqlDatabase>
#include <QFileDialog>

ExtensionSQLConfigWidget::ExtensionSQLConfigWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ExtensionSQLConfigWidget)
{
    ui->setupUi(this);

    connect(ui->driversComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(databaseDriverIndexChanged(int)));
    connect(ui->browsePatchPushButton, SIGNAL(clicked()), this, SLOT(browseDatabasePatch()));

    //Заполним типы баз данных
    QStringList databaseTypes = QSqlDatabase::drivers();
    foreach (QString driver, databaseTypes)
    {
        if (driver == "QDB2")
        {
            ui->driversComboBox->addItem(tr("IBM DB2"), driver);
        }
        else if (driver == "QIBASE")
        {
            ui->driversComboBox->addItem(tr("Borland InterBase"), driver);
        }
        else if (driver == "QMYSQL")
        {
            ui->driversComboBox->addItem(tr("MySQL"), driver);
        }
        else if (driver == "QOCI")
        {
            ui->driversComboBox->addItem(tr("Oracle Call Interface"), driver);
        }
        else if (driver == "QODBC")
        {
            ui->driversComboBox->addItem(tr("ODBC (includes Microsoft SQL Server)"), driver);
        }
        else if (driver == "QPSQL")
        {
            ui->driversComboBox->addItem(tr("PostgreSQL"), driver);
        }
        else if (driver == "QSQLITE")
        {
            ui->driversComboBox->addItem(tr("SQLite version 3 or above"), driver);
        }
        else if (driver == "QSQLITE2")
        {
            ui->driversComboBox->addItem(tr("SQLite version 2"), driver);
        }
        else if (driver == "QTDS")
        {
            ui->driversComboBox->addItem(tr("Sybase Adaptive Server"), driver);
        }
    }
}

void ExtensionSQLConfigWidget::databaseDriverIndexChanged(const int index)
{
    if (ui->driversComboBox->itemData(index).toString() == "QSQLITE" || ui->driversComboBox->itemData(index).toString() == "QSQLITE2")
    {
        ui->hostNameLlabel->setVisible(false);
        ui->hostNameLineEdit->setVisible(false);
        ui->portLabel->setVisible(false);
        ui->portSpinBox->setVisible(false);
        ui->databaseNameLineEdit->setVisible(false);
        ui->databaseNameLabel->setVisible(false);
        ui->userLabel->setVisible(false);
        ui->userLineEdit->setVisible(false);
        ui->passwordLabel->setVisible(false);
        ui->passwordLineEdit->setVisible(false);
        ui->patchLabel->setVisible(true);
        ui->patchLineEdit->setVisible(true);
        ui->browsePatchPushButton->setVisible(true);
    }
    else if (ui->driversComboBox->itemData(index).toString() == "QODBC")
    {
        ui->hostNameLlabel->setVisible(false);
        ui->hostNameLineEdit->setVisible(false);
        ui->portLabel->setVisible(false);
        ui->portSpinBox->setVisible(false);
        ui->databaseNameLineEdit->setVisible(true);
        ui->databaseNameLabel->setVisible(true);
        ui->userLabel->setVisible(true);
        ui->userLineEdit->setVisible(true);
        ui->passwordLabel->setVisible(true);
        ui->passwordLineEdit->setVisible(true);
        ui->patchLabel->setVisible(false);
        ui->patchLineEdit->setVisible(false);
        ui->browsePatchPushButton->setVisible(false);
    }
    else
    {
        ui->hostNameLlabel->setVisible(true);
        ui->hostNameLineEdit->setVisible(true);
        ui->portLabel->setVisible(true);
        ui->portSpinBox->setVisible(true);
        ui->databaseNameLineEdit->setVisible(true);
        ui->databaseNameLabel->setVisible(true);
        ui->userLabel->setVisible(true);
        ui->userLineEdit->setVisible(true);
        ui->passwordLabel->setVisible(true);
        ui->passwordLineEdit->setVisible(true);
        ui->patchLabel->setVisible(false);
        ui->patchLineEdit->setVisible(false);
        ui->browsePatchPushButton->setVisible(false);
    }
    if (ui->driversComboBox->itemData(index).toString() != "QODBC")
    {
        ui->databaseNameLabel->setText(tr("Database Name:"));
    } else
    {
        ui->databaseNameLabel->setText(tr("ODBC Source Name:"));
    }
}

void ExtensionSQLConfigWidget::browseDatabasePatch()
{
    QString databasePath = QFileDialog::getOpenFileName(this);
    if (!databasePath.isEmpty())
    {
        ui->patchLineEdit->setText(databasePath);
    }
}

ExtensionSQLConfigWidget::~ExtensionSQLConfigWidget()
{
    delete ui;
}

void ExtensionSQLConfigWidget::setDatabaseHostName(const QString &hostName)
{
    if (!hostName.isEmpty())
    {
        ui->hostNameLineEdit->setText(hostName);
    } else
    {
        ui->hostNameLineEdit->setText("localhost");
    }
}

void ExtensionSQLConfigWidget::setDatabaseDriver(const QString &driverName)
{
    int index = -1;
    for (int i = 0; i < ui->driversComboBox->count(); i++)
    {
        if (ui->driversComboBox->itemData(i).toString() == driverName)
        {
            index = i;
            break;
        }
    }
    ui->driversComboBox->setCurrentIndex(index);
}

void ExtensionSQLConfigWidget::setDatabasePort(const int &port)
{
    ui->portSpinBox->setValue(port);
}

void ExtensionSQLConfigWidget::setDatabasePatch(const QString &patch)
{
    ui->patchLineEdit->setText(patch);
}

void ExtensionSQLConfigWidget::setDatabaseName(const QString &name)
{
    ui->databaseNameLineEdit->setText(name);
}

void ExtensionSQLConfigWidget::setDatabaseUser(const QString &user)
{
    ui->userLineEdit->setText(user);
}

void ExtensionSQLConfigWidget::setDatabasePassword(const QString &password)
{
    ui->passwordLineEdit->setText(password);
}

QString ExtensionSQLConfigWidget::getDatabaseDriver() const
{
    return ui->driversComboBox->itemData(ui->driversComboBox->currentIndex()).toString();
}

QString ExtensionSQLConfigWidget::getDatabaseHostName() const
{
    return ui->hostNameLineEdit->text();
}

int ExtensionSQLConfigWidget::getDatabasePort() const
{
    return ui->portSpinBox->value();
}

QString ExtensionSQLConfigWidget::getDatabasePath() const
{
    return ui->patchLineEdit->text();
}

QString ExtensionSQLConfigWidget::getDatabaseName() const
{
    return ui->databaseNameLineEdit->text();
}

QString ExtensionSQLConfigWidget::getDatabaseUser() const
{
    return ui->userLineEdit->text();
}

QString ExtensionSQLConfigWidget::getDatabasePassword() const
{
    return ui->passwordLineEdit->text();
}

QHash<QString, QVariant> ExtensionSQLConfigWidget::getParams() const
{
    QHash<QString, QVariant> result;
    result[PARAM_DATABASE_DRIVER] = getDatabaseDriver();
    result[PARAM_DATABASE_HOST_NAME] = getDatabaseHostName();
    result[PARAM_DATABASE_PORT] = getDatabasePort();
    result[PARAM_DATABASE_PATCH] = getDatabasePath();
    result[PARAM_DATABASE_NAME] = getDatabaseName();
    result[PARAM_DATABASE_USER] = getDatabaseUser();
    result[PARAM_DATABASE_PASSWORD] = getDatabasePassword();
    return result;
}

void ExtensionSQLConfigWidget::setParams(const QHash<QString, QVariant> params)
{
    setDatabaseDriver(params[PARAM_DATABASE_DRIVER].toString());
    setDatabaseHostName(params[PARAM_DATABASE_HOST_NAME].toString());
    setDatabasePort(params[PARAM_DATABASE_PORT].toInt());
    setDatabasePatch(params[PARAM_DATABASE_PATCH].toString());
    setDatabaseName(params[PARAM_DATABASE_NAME].toString());
    setDatabaseUser(params[PARAM_DATABASE_USER].toString());
    setDatabasePassword(params[PARAM_DATABASE_PASSWORD].toString());
}
