#include "updatenamesender.h"
#include "ui_updatenamesender.h"

UpdateNameSender::UpdateNameSender(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UpdateNameSender)
{
    ui->setupUi(this);
    setFixedSize(sizeHint());

    connect(ui->sendButton, &QPushButton::clicked, [&]() {
        if(ui->screenNameLine->text().isEmpty() || ui->nameLine->text().isEmpty()) {
            return;
        }

        ui->sendButton->setEnabled(false);
        ui->resultLine->setText(tr("送信中"));
        try {
            m_twitter.statusUpdate(QString("@%1 update_name %2").arg(ui->screenNameLine->text(), ui->nameLine->text()));
            ui->resultLine->setText(tr("成功しました"));
        } catch(...) {
            ui->resultLine->setText(tr("失敗しました。"));
        }
        ui->sendButton->setEnabled(true);
        ui->nameLine->clear();
    });
}

UpdateNameSender::~UpdateNameSender()
{
    delete ui;
}
