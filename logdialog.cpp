#include "logdialog.h"
#include "ui_logdialog.h"
#include "mdvplayerapplication.h"
#include <QDebug>

LogDialog::LogDialog(QWidget *parent, Qt::WindowFlags f) :
    QDialog(parent, f),
    ui(new Ui::LogDialog)
{
    ui->setupUi(this);
    ui->tableView->setModel(MDVPlayerApplication::logModel());
    ui->tableView->horizontalHeader()->setResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setResizeMode(1, QHeaderView::Stretch);
    ui->tableView->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
}

LogDialog::~LogDialog()
{
    delete ui;
}
