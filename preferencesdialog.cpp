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

    ui->tab->setCurrentIndex(0);

    ui->selectMessageBox->addItems(QStringList()
                                   << tr("開始/終了")
                                   << tr("update_name")
                                   << tr("update_url")
                                   << tr("update_location")
                                   << tr("update_description")
                                   << tr("update_image"));
    ui->selectMessageBox->setCurrentIndex(0);
    ui->messageStack->setCurrentIndex(0);

    ui->settingsFilePathLine->setText(m_settings.filePath());
    
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
            m_settings.setConsumerKey(NULL);
            m_settings.setConsumerSecret(NULL);
            m_settings.setAccessToken(NULL);
            m_settings.setAccessTokenSecret(NULL);
            QApplication::exit(255);
        }
    });
    connect(ui->logoutButton, &QPushButton::clicked, [&]() {
        if(QMessageBox::question(this,
                                 tr("確認"),
                                 tr("プログラムを終了します\n"
                                    "よろしいですか？"),
                                 QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes) {
            m_settings.setConsumerKey(NULL);
            m_settings.setConsumerSecret(NULL);
            m_settings.setAccessToken(NULL);
            m_settings.setAccessTokenSecret(NULL);
            QApplication::quit();
        }
    });
    connect(ui->openSettingsFile, &QPushButton::clicked, [&]() {
        QDesktopServices::openUrl(QUrl(m_settings.filePath()));
    });
    connect(ui->removeSettingsFile, &QPushButton::clicked, [&]() {
        QMessageBox rm_msg(QMessageBox::Question,
                           tr("確認"),
                           tr("設定ファイルを消しますよ？"),
                           QMessageBox::Yes | QMessageBox::No,
                           this);
        if(rm_msg.exec() == QMessageBox::Yes) {
            QFile file(m_settings.filePath());
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
}

PreferencesDialog::~PreferencesDialog()
{
    delete ui;
}

void PreferencesDialog::showEvent(QShowEvent *)
{
    loadSettings();
}

void PreferencesDialog::closeEvent(QCloseEvent *)
{
    saveSettings();
}

void PreferencesDialog::loadSettings()
{
    /* 動作 */
    ui->stayOnSystemTrayCheck->setChecked(m_settings.isStayOnSystemTray());
    ui->autoStartUpdateNameCheck->setChecked(m_settings.isAutoStartUpdateName());

    /* コマンド */
    ui->enabledUpdateNameCheck->setChecked(m_settings.isEnabledUpdateName());
    ui->updateNameFormatLine->setText(m_settings.updateNameFormat());
    ui->enabledUpdateUrlCheck->setChecked(m_settings.isEnabledUpdateUrl());
    ui->enabledUpdateLocationCheck->setChecked(m_settings.isEnabledUpdateLocation());
    ui->enabledUpdateDescriptionCheck->setChecked(m_settings.isEnabledUpdateDescription());
    ui->enabledUpdateImageCheck->setChecked(m_settings.isEnabledUpdateImage());

    /* メッセージ */
    //開始/終了
    ui->postStartupMessageCheck->setChecked(m_settings.isPostStartupMessage());
    ui->startupMessageText->setPlainText(m_settings.startupMessage());
    ui->postClosedMessageCheck->setChecked(m_settings.isPostClosedMessage());
    ui->closedMessageText->setPlainText(m_settings.closedMessage());

    //update_name
    ui->postUpdateNameSuccessedMessageCheck->setChecked(m_settings.isPostUpdateNameSuccessedMessage());
    ui->updateNameSuccessedMessageText->setPlainText(m_settings.updateNameSuccessedMessage());
    ui->postUpdateNameFailedMessageCheck->setChecked(m_settings.isPostUpdateNameFailedMessage());
    ui->updateNameFailedMessageText->setPlainText(m_settings.updateNameFailedMessage());

    //update_url
    ui->postUpdateUrlSuccessedMessageCheck->setChecked(m_settings.isPostUpdateUrlSuccessedMessage());
    ui->updateUrlSuccessedMessageText->setPlainText(m_settings.updateUrlSuccessedMessage());
    ui->postUpdateUrlFailedMessageCheck->setChecked(m_settings.isPostUpdateUrlFailedMessage());
    ui->updateUrlFailedMessageText->setPlainText(m_settings.updateUrlFailedMessage());

    //update_location
    ui->postUpdateLocationSuccessedMessageCheck->setChecked(m_settings.isPostUpdateLocationSuccessedMessage());
    ui->updateLocationSuccessedMessageText->setPlainText(m_settings.updateLocationSuccessedMessage());
    ui->postUpdateLocationFailedMessageCheck->setChecked(m_settings.isPostUpdateLocationFailedMessage());
    ui->updateLocationFailedMessageText->setPlainText(m_settings.updateLocationFailedMessage());

    //update_description
    ui->postUpdateDescriptionSuccessedMessageCheck->setChecked(m_settings.isPostUpdateDescriptionSuccessedMessage());
    ui->updateDescriptionSuccessedMessageText->setPlainText(m_settings.updateDescriptionSuccessedMessage());
    ui->postUpdateDescriptionFailedMessageCheck->setChecked(m_settings.isPostUpdateDescriptionFailedMessage());
    ui->updateDescriptionFailedMessageText->setPlainText(m_settings.updateDescriptionFailedMessage());

    //update_image
    ui->postUpdateImageSuccessedMessageCheck->setChecked(m_settings.isPostUpdateImageSuccessedMessage());
    ui->updateImageSuccessedMessageText->setPlainText(m_settings.updateImageSuccessedMessage());
    ui->postUpdateImageFailedMessageCheck->setChecked(m_settings.isPostUpdateImageFailedMessage());
    ui->updateImageFailedMessageText->setPlainText(m_settings.updateImageFailedMessage());
}

void PreferencesDialog::saveSettings()
{
    /* 動作 */
    m_settings.setIsStayOnSystemTray(ui->stayOnSystemTrayCheck->isChecked());
    m_settings.setIsAutoStartUpdateName(ui->autoStartUpdateNameCheck->isChecked());

    /* コマンド */
    m_settings.setUpdateNameEnabled(ui->enabledUpdateNameCheck->isChecked());
    m_settings.setUpdateNameFormat(ui->updateNameFormatLine->text());
    m_settings.setUpdateUrlEnabled(ui->enabledUpdateUrlCheck->isChecked());
    m_settings.setUpdateLocationEnabled(ui->enabledUpdateLocationCheck->isChecked());
    m_settings.setUpdateDescriptionEnabled(ui->enabledUpdateDescriptionCheck->isChecked());
    m_settings.setUpdateImageEnabled(ui->enabledUpdateImageCheck->isChecked());
    
    /* メッセージ */
    //開始/終了
    m_settings.setIsPostStartupMessage(ui->postStartupMessageCheck->isChecked());
    m_settings.setStartupMessage(ui->startupMessageText->toPlainText());
    m_settings.setIsPostClosedMessage(ui->postClosedMessageCheck->isChecked());
    m_settings.setClosedMessage(ui->closedMessageText->toPlainText());

    //update_name
    m_settings.setIsPostUpdateNameSuccessedMessage(ui->postUpdateNameSuccessedMessageCheck->isChecked());
    m_settings.setUpdateNameSuccessedMessage(ui->updateNameSuccessedMessageText->toPlainText());
    m_settings.setIsPostUpdateNameFailedMessage(ui->postUpdateNameFailedMessageCheck->isChecked());
    m_settings.setUpdateNameFailedMessage(ui->updateNameFailedMessageText->toPlainText());

    //update_url
    m_settings.setIsPostUpdateUrlSuccessedMessage(ui->postUpdateUrlSuccessedMessageCheck->isChecked());
    m_settings.setUpdateUrlSuccessedMessage(ui->updateUrlSuccessedMessageText->toPlainText());
    m_settings.setIsPostUpdateUrlFailedMessage(ui->postUpdateUrlFailedMessageCheck->isChecked());
    m_settings.setUpdateUrlFailedMessage(ui->updateUrlFailedMessageText->toPlainText());

    //update_location
    m_settings.setIsPostUpdateLocationSuccessedMessage(ui->postUpdateLocationSuccessedMessageCheck->isChecked());
    m_settings.setUpdateLocationSuccessedMessage(ui->updateLocationSuccessedMessageText->toPlainText());
    m_settings.setIsPostUpdateLocationFailedMessage(ui->postUpdateLocationFailedMessageCheck->isChecked());
    m_settings.setUpdateLocationFailedMessage(ui->updateLocationFailedMessageText->toPlainText());

    //update_description
    m_settings.setIsPostUpdateDescriptionSuccessedMessage(ui->postUpdateDescriptionSuccessedMessageCheck->isChecked());
    m_settings.setUpdateDescriptionSuccessedMessage(ui->updateDescriptionSuccessedMessageText->toPlainText());
    m_settings.setIsPostUpdateDescriptionFailedMessage(ui->postUpdateDescriptionFailedMessageCheck->isChecked());
    m_settings.setUpdateDescriptionFailedMessage(ui->updateDescriptionFailedMessageText->toPlainText());

    //update_image
    m_settings.setIsPostUpdateImageSuccessedMessage(ui->postUpdateImageSuccessedMessageCheck->isChecked());
    m_settings.setUpdateImageSuccessedMessage(ui->updateImageSuccessedMessageText->toPlainText());
    m_settings.setIsPostUpdateImageFailedMessage(ui->postUpdateImageFailedMessageCheck->isChecked());
    m_settings.setUpdateImageFailedMessage(ui->updateImageFailedMessageText->toPlainText());
}
