#include "preferencesdialog.h"
#include "ui_preferencesdialog.h"
#include "updatenameqtglobal.h"

#include <QDesktopServices>
#include <QUrl>
#include <QMessageBox>
#include <QFile>

const QSize PreferencesDialog::ICON_SIZE(20, 20);

PreferencesDialog::PreferencesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PreferencesDialog)
{
    ui->setupUi(this);

    //アイコンのセット
    ui->actionButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_CommandLink));
    ui->commandButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_TitleBarMenuButton));
    ui->messageButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_MessageBoxInformation));
    ui->advancedSettingButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_MessageBoxWarning));
    //アイコンの大きさのセット
    for (QPushButton *button : ui->scrollAreaWidgetContents->findChildren<QPushButton *>())
        button->setIconSize(ICON_SIZE);
    //コマンド名のセット
    ui->commandBox->addItems(UpdateNameQt::updateCommands);


    //設定ファイルの場所のセット
    ui->settingsFileNameLine->setText(UpdateNameQt::settings->fileName());
    //設定ファイルの内容のセット
    ui->textBrowser->setSource(QUrl(UpdateNameQt::settings->fileName()));

    connect(ui->actionButton, &QPushButton::clicked, [&]() {
        ui->stackedWidget->setCurrentIndex(0);
    });
    connect(ui->commandButton, &QPushButton::clicked, [&]() {
        ui->stackedWidget->setCurrentIndex(1);
    });
    connect(ui->messageButton, &QPushButton::clicked, [&]() {
        ui->stackedWidget->setCurrentIndex(2);
    });
    connect(ui->advancedSettingButton, &QPushButton::clicked, [&]() {
        ui->stackedWidget->setCurrentIndex(3);
    });
    connect(ui->openSettingsFileButton, &QPushButton::clicked, [&]() {
        QDesktopServices::openUrl(QUrl(UpdateNameQt::settings->fileName()));
    });
    connect(ui->deleteSettingsFileButton, &QPushButton::clicked, [&]() {
        QFile file(UpdateNameQt::settings->fileName());
        if (QMessageBox::question(this, tr("確認"), tr("設定ファイルを削除します。\nよろしいですか？"), QMessageBox::Yes, QMessageBox::No)
            == QMessageBox::Yes) {
            if (file.remove()) {
                QMessageBox::information(this, tr("削除完了"), tr("設定ファイルを削除しました。\nアプリケーションを再起動します。"), QMessageBox::Ok);
                QApplication::exit(UpdateNameQt::ExitRestart);
                return;
            }
        }
        QMessageBox::critical(this, tr("エラー"), tr("設定ファイルの削除に失敗しました。\n%1").arg(file.errorString()), QMessageBox::Ok);
    });
}

PreferencesDialog::~PreferencesDialog()
{
    delete ui;
}
