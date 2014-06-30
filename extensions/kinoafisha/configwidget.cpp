#include "kinoafishadefines.h"

#include "configwidget.h"
#include "ui_configwidget.h"
#include "../extensionsqlconfigwidget.h"
#include <QMessageBox>

ConfigWidget::ConfigWidget(QWidget *parent) :
    AbstractExtentionConfigWidget(parent),
    ui(new Ui::ConfigWidget)
{
    ui->setupUi(this);
    connect(ui->cinemaListHelpToolButton, SIGNAL(clicked()), this, SLOT(showHelp()));
    connect(ui->cinemaPriceHelpToolButton, SIGNAL(clicked()), this, SLOT(showHelp()));
}

ConfigWidget::~ConfigWidget()
{
    delete ui;
}

QHash<QString, QVariant> ConfigWidget::getParams() const
{
    QHash<QString, QVariant> params;
    params = ui->sqlConfigWidget->getParams();
    params[PARAM_CINEMA_LIST_QUERY] = ui->cinemaListTextEdit->document()->toPlainText();
    params[PARAM_CINEMA_PRICE_QUERY] = ui->cinemaPricePlainTextEdit->document()->toPlainText();
    params[PARAM_FREE_PLACE_UPDATE_INTERVAL] = ui->freePlaceUpdateIntervalSpinBox->value();
    params[PARAM_RELOAD_KINOAFISHA_TIME] = ui->updateTimeEdit->time();
    return params;
}

void ConfigWidget::setParams(const QHash<QString, QVariant> params)
{
    ui->sqlConfigWidget->setParams(params);
    ui->cinemaListTextEdit->document()->setPlainText(params[PARAM_CINEMA_LIST_QUERY].toString());
    ui->cinemaPricePlainTextEdit->document()->setPlainText(params[PARAM_CINEMA_PRICE_QUERY].toString());
    if (params[PARAM_FREE_PLACE_UPDATE_INTERVAL].toInt() != 0)
    {
        ui->freePlaceUpdateIntervalSpinBox->setValue(params[PARAM_FREE_PLACE_UPDATE_INTERVAL].toInt());
    } else
    {
        ui->freePlaceUpdateIntervalSpinBox->setValue(30);
    }
    if (params[PARAM_RELOAD_KINOAFISHA_TIME].toTime().isValid())
    {
        ui->updateTimeEdit->setTime(params[PARAM_RELOAD_KINOAFISHA_TIME].toTime());
    } else
    {
        ui->updateTimeEdit->setTime(QTime(4, 00));
    }
}

void ConfigWidget::showHelp()
{
    QToolButton *senderButton = qobject_cast<QToolButton *>(sender());
    if (senderButton == ui->cinemaListHelpToolButton)
    {
        QMessageBox::information(this, tr("Cinema List Help"), tr("This request will be made to the database specified in the "
                                                                  "settings of the database to get a list of cinema. The result "
                                                                  "of this query should be a sample that includes all sessions "
                                                                  "yesterday, today and tomorrow.\n\n "
                                                                  "The result should contain the following required fields:\n"
                                                                  "- ID - unique identifier of the movie\n;"
                                                                  "- NAME - the name of the film show;\n"
                                                                  "- HALLNAME - name of the hall, which will be conducted Picture Show;\n"
                                                                  "- STARTDATETIME - date and time of the screening, in a standard format, SQL;\n"
                                                                  "- DURATION - the time (in minutes) How long is the film show;\n"
                                                                  "- FREEPLACE - surprising number of empty seats at the cinema "
                                                                  "show. If Number is 0, it is considered that the tickets for "
                                                                  "the film is not, unless you want ispolzovovat display function "
                                                                  "Number of free tickets, then this field should always be -1;\n"
                                                                  "- IS3D - a sign that the three-dimensional film. Maybe true "
                                                                  "and false, or 1 and 0.\n"
                                                                  "- AGELIMIT - age limit of cinema.\n\n"
                                                                  "Example SQL-query:\n"
                                                                  "SELECT `SHOWING`. `Name` AS `NAME`, `start` AS `STARTDATETIME`, "
                                                                  "`duration` AS `DURATION`, `freePlace` AS `FREEPLACE`, `is3D` "
                                                                  "AS `IS3D`, `HALLS`. `name` AS `HALLNAME` FROM `SHOWING`, "
                                                                  "`HALLS` WHERE `SHOWING`.`hallId` = `HALLS`.`id` AND "
                                                                  "`start` < ADDDATE (CURDATE (), INTERVAL + 1 DAY) AND `start` > "
                                                                  "ADDDATE (CURDATE (), INTERVAL - 1 DAY)"));
    }
    else if (senderButton == ui->cinemaPriceHelpToolButton)
    {
        QMessageBox::information(this, tr("Cinema Price Help"), tr("This request will be made to the database specified in the "
                                                                   "settings of the database for a list of prices for the "
                                                                   "specified session. The result of this query should be a "
                                                                   "sample that contains all of the prices for the specified "
                                                                   "session.\n\n"
                                                                   "The result should contain the following required fields:\n"
                                                                   "- PRICE - the price of the specified session;\n\n"
                                                                   "The request can be used zabindenye following parameters:\n"
                                                                   "-:cinemaId - the unique identifier of the film show;\n\n"
                                                                   "Example SQL-query:\n"
                                                                   "SELECT `price` AS `PRICE` FROM `SHOWING`, `PRICES` "
                                                                   "WHERE `PRICES`. `FilmId` = `SHOWING`.`id` AND `SHOWING`.`id` =:cinemaId"));

    }
}
