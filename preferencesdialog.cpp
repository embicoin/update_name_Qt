#include "preferencesdialog.h"
#include "ui_preferencesdialog.h"

#include <QDesktopServices>
#include <QUrl>
#include <QMessageBox>

PreferencesDialog::PreferencesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PreferencesDialog)
{
    ui->setupUi(this);

    setFixedSize(size());

    ui->selectMessageBox->addItems(QStringList()
                                   << tr("開始/終了")
                                   << tr("update_name")
                                   << tr("update_url")
                                   << tr("update_location")
                                   << tr("update_description"));

    ui->settingsFilePathLine->setText(settings.filePath());
    
    connect(ui->selectMessageBox, SIGNAL(currentIndexChanged(int)), ui->messageStack, SLOT(setCurrentIndex(int)));
    connect(ui->startupMessageText, &QTextEdit::textChanged, [&]() {
        ui->startupMessageLengthCountLabel->setText(QString::number(ui->startupMessageText->toPlainText().length()));
    });
    connect(ui->closedMessageText, &QTextEdit::textChanged, [&]() {
        ui->closedMessageLengthCountLabel->setText(QString::number(ui->closedMessageText->toPlainText().length()));
    });
    connect(ui->reAuthButton, &QPushButton::clicked, [&]() {
        if(QMessageBox::question(this,
                                 tr("確認"),
                                 tr("プログラムを再起動します\n"
                                    "よろしいですか？"),
                                 QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes) {
            settings.setConsumerKey(NULL);
            settings.setConsumerSecret(NULL);
            settings.setAccessToken(NULL);
            settings.setAccessTokenSecret(NULL);
            QApplication::exit(255);
        }
    });
    connect(ui->logoutButton, &QPushButton::clicked, [&]() {
        if(QMessageBox::question(this,
                                 tr("確認"),
                                 tr("プログラムを終了します\n"
                                    "よろしいですか？"),
                                 QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes) {
            settings.setConsumerKey(NULL);
            settings.setConsumerSecret(NULL);
            settings.setAccessToken(NULL);
            settings.setAccessTokenSecret(NULL);
            QApplication::quit();
        }
    });
    connect(ui->openSettingsFile, &QPushButton::clicked, [&]() {
        QDesktopServices::openUrl(QUrl(settings.filePath()));
    });
    connect(ui->removeSettingsFile, &QPushButton::clicked, [&]() {
        QMessageBox rm_msg(QMessageBox::Question,
                           tr("確認"),
                           tr("設定ファイルを消しますよ？"),
                           QMessageBox::Yes | QMessageBox::No,
                           this);
        if(rm_msg.exec() == QMessageBox::Yes) {
            QFile file(settings.filePath());
            file.remove();
            exit(0);
        }
    });
    connect(ui->buttonBox, &QDialogButtonBox::clicked, [&](QAbstractButton *button) {
        switch(ui->buttonBox->standardButton(button)) {
        case QDialogButtonBox::Apply:
            saveSettings();
            break;
        case QDialogButtonBox::Ok:
            saveSettings();
            accept();
            break;
        case QDialogButtonBox::Cancel:
            reject();
            break;
        default:
            break;
        }
    });

    loadSettings();
}

PreferencesDialog::~PreferencesDialog()
{
    delete ui;
}

void PreferencesDialog::loadSettings()
{
    /* 動作 */
    ui->stayOnSystemTrayCheck->setChecked(settings.isStayOnSystemTray());
    ui->autoStartUpdateNameCheck->setChecked(settings.isAutoStartUpdateName());

    /* コマンド */
    ui->enabledUpdateNameCheck->setChecked(settings.isEnabledUpdateName());
    ui->enabledUpdateUrlCheck->setChecked(settings.isEnabledUpdateUrl());
    ui->enabledUpdateLocationCheck->setChecked(settings.isEnabledUpdateLocation());
    ui->enabledUpdateDescriptionCheck->setChecked(settings.isEnabledUpdateDescription());

    /* メッセージ */
    //開始/終了
    ui->postStartupMessageCheck->setChecked(settings.isPostStartupMessage());
    ui->startupMessageText->setPlainText(settings.startupMessage());
    ui->postClosedMessageCheck->setChecked(settings.isPostClosedMessage());
    ui->closedMessageText->setPlainText(settings.closedMessage());

    //update_name
    ui->postUpdateNameSuccessedMessageCheck->setChecked(settings.isPostUpdateNameSuccessedMessage());
    ui->updateNameSuccessedMessageText->setPlainText(settings.updateNameSuccessedMessage());
    ui->postUpdateNameFailedMessageCheck->setChecked(settings.isPostUpdateNameFailedMessage());
    ui->updateNameFailedMessageText->setPlainText(settings.updateNameFailedMessage());

    //update_url
    ui->postUpdateUrlSuccessedMessageCheck->setChecked(settings.isPostUpdateUrlSuccessedMessage());
    ui->updateUrlSuccessedMessageText->setPlainText(settings.updateUrlSuccessedMessage());
    ui->postUpdateUrlFailedMessageCheck->setChecked(settings.isPostUpdateUrlFailedMessage());
    ui->updateUrlFailedMessageText->setPlainText(settings.updateUrlFailedMessage());

    //update_location
    ui->postUpdateLocationSuccessedMessageCheck->setChecked(settings.isPostUpdateLocationSuccessedMessage());
    ui->updateLocationSuccessedMessageText->setPlainText(settings.updateLocationSuccessedMessage());
    ui->postUpdateLocationFailedMessageCheck->setChecked(settings.isPostUpdateLocationFailedMessage());
    ui->updateLocationFailedMessageText->setPlainText(settings.updateLocationFailedMessage());

    //update_description
    ui->postUpdateDescriptionSuccessedMessageCheck->setChecked(settings.isPostUpdateDescriptionSuccessedMessage());
    ui->updateDescriptionSuccessedMessageText->setPlainText(settings.updateDescriptionSuccessedMessage());
    ui->postUpdateDescriptionFailedMessageCheck->setChecked(settings.isPostUpdateDescriptionFailedMessage());
    ui->updateDescriptionFailedMessageText->setPlainText(settings.updateDescriptionFailedMessage());
}

void PreferencesDialog::saveSettings()
{
    /* 動作 */
    settings.setIsStayOnSystemTray(ui->stayOnSystemTrayCheck->isChecked());
    settings.setIsAutoStartUpdateName(ui->autoStartUpdateNameCheck->isChecked());

    /* コマンド */
    settings.setUpdateNameEnabled(ui->enabledUpdateNameCheck->isChecked());
    settings.setUpdateUrlEnabled(ui->enabledUpdateUrlCheck->isChecked());
    settings.setUpdateLocationEnabled(ui->enabledUpdateLocationCheck->isChecked());
    settings.setUpdateDescriptionEnabled(ui->enabledUpdateDescriptionCheck->isChecked());
    
    /* メッセージ */
    //開始/終了
    settings.setIsPostStartupMessage(ui->postStartupMessageCheck->isChecked());
    settings.setStartupMessage(ui->startupMessageText->toPlainText());
    settings.setIsPostClosedMessage(ui->postClosedMessageCheck->isChecked());
    settings.setClosedMessage(ui->closedMessageText->toPlainText());

    //update_name
    settings.setIsPostUpdateNameSuccessedMessage(ui->postUpdateNameSuccessedMessageCheck->isChecked());
    settings.setUpdateNameSuccessedMessage(ui->updateNameSuccessedMessageText->toPlainText());
    settings.setIsPostUpdateNameFailedMessage(ui->postUpdateNameFailedMessageCheck->isChecked());
    settings.setUpdateNameFailedMessage(ui->updateNameFailedMessageText->toPlainText());

    //update_url
    settings.setIsPostUpdateUrlSuccessedMessage(ui->postUpdateUrlSuccessedMessageCheck->isChecked());
    settings.setUpdateUrlSuccessedMessage(ui->updateNameSuccessedMessageText->toPlainText());
    settings.setIsPostUpdateUrlFailedMessage(ui->postUpdateUrlFailedMessageCheck->isChecked());
    settings.setUpdateUrlFailedMessage(ui->updateUrlFailedMessageText->toPlainText());

    //update_location
    settings.setIsPostUpdateLocationSuccessedMessage(ui->postUpdateLocationSuccessedMessageCheck->isChecked());
    settings.setUpdateLocationSuccessedMessage(ui->updateLocationSuccessedMessageText->toPlainText());
    settings.setIsPostUpdateLocationFailedMessage(ui->postUpdateLocationFailedMessageCheck->isChecked());
    settings.setUpdateLocationFailedMessage(ui->updateLocationFailedMessageText->toPlainText());

    //update_description
    settings.setIsPostUpdateDescriptionSuccessedMessage(ui->postUpdateDescriptionSuccessedMessageCheck->isChecked());
    settings.setUpdateDescriptionSuccessedMessage(ui->updateDescriptionSuccessedMessageText->toPlainText());
    settings.setIsPostUpdateDescriptionFailedMessage(ui->postUpdateDescriptionFailedMessageCheck->isChecked());
    settings.setUpdateDescriptionFailedMessage(ui->updateDescriptionFailedMessageText->toPlainText());
}
