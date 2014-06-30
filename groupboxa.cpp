#include "groupboxa.h"
#include "ui_groupboxa.h"

GroupBoxa::GroupBoxa(QWidget *parent) :
    QGroupBox(parent),
    ui(new Ui::GroupBoxa)
{
    ui->setupUi(this);
}

GroupBoxa::~GroupBoxa()
{
    delete ui;
}
