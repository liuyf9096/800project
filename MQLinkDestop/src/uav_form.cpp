#include "uav_form.h"
#include "ui_uav_form.h"

UAVForm::UAVForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::UAVForm)
{
    ui->setupUi(this);
    ui->label_address->clear();
    ui->label_id->clear();
}

UAVForm::~UAVForm()
{
    delete ui;
}

void UAVForm::setIpAddress(QString ip)
{
    ui->label_address->setText(ip);
}

void UAVForm::setDeviceId(QString id)
{
    ui->label_id->setText(id);
}
