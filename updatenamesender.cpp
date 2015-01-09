#include "updatenamesender.h"
#include "ui_updatenamesender.h"

UpdateNameSender::UpdateNameSender(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UpdateNameSender)
{
    ui->setupUi(this);
}

UpdateNameSender::~UpdateNameSender()
{
    delete ui;
}
