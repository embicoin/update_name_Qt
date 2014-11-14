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

    setFixedSize(sizeHint());

    ui->settingsFilePathLine->setText(settings.filePath());
    
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
    ui->stayOnSystemTrayCheck->setChecked(settings.isStayOnSystemTray());
    ui->autoStartUpdateNameCheck->setChecked(settings.isAutoStartUpdateName());

    ui->postStartupMessageCheck->setChecked(settings.isPostStartupMessage());
    ui->startupMessageText->setPlainText(settings.startupMessage());
    ui->postClosedMessageCheck->setChecked(settings.isPostClosedMessage());
    ui->closedMessageText->setPlainText(settings.closedMessage());
    ui->postUpdateNameSuccessedMessageCheck->setChecked(settings.isPostUpdateNameSuccessedMessage());
    ui->updateNameSuccessedMessageText->setPlainText(settings.updateNameSuccessedMessage());
    ui->postUpdateNameFailedMessage->setChecked(settings.isPostUpdateNameFailedMessage());
    ui->updateNameFailedMessageText->setPlainText(settings.updateNameFailedMessage());
}

void PreferencesDialog::saveSettings()
{
    settings.setIsStayOnSystemTray(ui->stayOnSystemTrayCheck->isChecked());
    settings.setIsAutoStartUpdateName(ui->autoStartUpdateNameCheck->isChecked());
    
    settings.setIsPostStartupMessage(ui->postStartupMessageCheck->isChecked());
    settings.setStartupMessage(ui->startupMessageText->toPlainText());
    settings.setIsPostClosedMessage(ui->postClosedMessageCheck->isChecked());
    settings.setClosedMessage(ui->closedMessageText->toPlainText());
    settings.setIsPostUpdateNameSuccessedMessage(ui->postUpdateNameSuccessedMessageCheck->isChecked());
    settings.setUpdateNameSuccessedMessage(ui->updateNameSuccessedMessageText->toPlainText());
    settings.setIsPostUpdateNameFailedMessage(ui->postUpdateNameFailedMessage->isChecked());
    settings.setUpdateNameFailedMessage(ui->updateNameFailedMessageText->toPlainText());
}
