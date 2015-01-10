#include "updatenamesender.h"
#include "ui_updatenamesender.h"
#include "updatenameqtglobal.h"
#include "twitterAPI/rest/statuses/update.h"

#include <QMessageBox>
#include <QThread>

using UpdateNameQt::settings;

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

void UpdateNameSender::on_tweetButton_clicked()
{
    if (ui->screenNameLine->text().isEmpty()) {
        QMessageBox::critical(this, tr("エラー"), tr("screen_nameが空です。"), QMessageBox::Ok);
        return;
    } else if (ui->valueText->toPlainText().isEmpty()) {
        QMessageBox::critical(this, tr("エラー"), tr("値が空です。"), QMessageBox::Ok);
        return;
    }

    const QString screenName = ui->screenNameLine->text().remove(QRegExp("^@"));
    const QString command = ui->commandLine->text().isEmpty() ? "update_name" : ui->commandLine->text();
    const QStringList valueList = ui->valueText->toPlainText().split("\n");

    for (QString value : valueList) {
        auto *statusUpdate = new TwitterAPI::Rest::Statuses::Update(TwitterAPI::OAuth(settings->value("ConsumerKey").toString(),
                                                                                      settings->value("ConsumerSecret").toString(),
                                                                                      settings->value("AccessToken").toString(),
                                                                                      settings->value("AccessTokenSecret").toString()));
        auto *statusUpdateThread = new QThread;

        connect(statusUpdate, SIGNAL(finished()), statusUpdateThread, SLOT(quit()));
        connect(statusUpdateThread, SIGNAL(finished()), statusUpdateThread, SLOT(deleteLater()));

        statusUpdate->moveToThread(statusUpdateThread);
        statusUpdateThread->start();
        statusUpdate->exec(QString("@%1 %2 %3").arg(screenName, command, value));
    }
}
