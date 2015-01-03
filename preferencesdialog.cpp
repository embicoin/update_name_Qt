#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif

#include "preferencesdialog.h"
#include "ui_preferencesdialog.h"
#include "updatenameqtglobal.h"

#include <QDesktopServices>
#include <QUrl>
#include <QMessageBox>
#include <QFile>
#include <QDebug>

const QSize PreferencesDialog::ICON_SIZE(20, 20);

using UpdateNameQt::settings;

PreferencesDialog::PreferencesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PreferencesDialog)
{
    ui->setupUi(this);
//#ifdef Q_OS_ANDROID
//    //ウィンドウの最大化
//    setWindowState(windowState() | Qt::WindowMaximized);
//    //改行
//    for (QLabel *label : findChildren<QLabel *>())
//        label->setWordWrap(true);
//#endif

#ifndef Q_OS_ANDROID
    //アイコンのセット
    ui->actionButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_CommandLink));
    ui->commandButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_TitleBarMenuButton));
    ui->messageButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_MessageBoxInformation));
    ui->authButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_DialogYesButton));
    ui->advancedSettingButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_MessageBoxWarning));
    //アイコンの大きさのセット
    for (QPushButton *button : ui->scrollAreaWidgetContents->findChildren<QPushButton *>())
        button->setIconSize(ICON_SIZE);
#endif
    //1ページ目を選択させる
    ui->stackedWidget->setCurrentIndex(0);
    //コマンド名のセット
    ui->commandBox->addItems(UpdateNameQt::updateCommands);
    //1ページ目を選択させる
    ui->commandStack->setCurrentIndex(0);
    //メッセージの種類のセット
    ui->messageBox->addItems(QStringList() << "起動/終了" << UpdateNameQt::updateCommands);
    //設定ファイルの場所のセット
    ui->settingsFileNameLine->setText(UpdateNameQt::settings->fileName());
    //設定ファイルの内容のセット
    ui->textBrowser->setSource(QUrl(UpdateNameQt::settings->fileName()));

    //connect
    //ページの切り替え
//#ifdef Q_OS_ANDROID
//    connect(ui->actionButton, &QPushButton::clicked, [&]() {
//        ui->stackedWidget->setCurrentIndex(1);
//    });
//    connect(ui->commandButton, &QPushButton::clicked, [&]() {
//        ui->stackedWidget->setCurrentIndex(2);
//    });
//    connect(ui->messageButton, &QPushButton::clicked, [&]() {
//        ui->stackedWidget->setCurrentIndex(3);
//    });
//    connect(ui->authButton, &QPushButton::clicked, [&]() {
//        ui->stackedWidget->setCurrentIndex(4);
//    });
//    connect(ui->advancedSettingButton, &QPushButton::clicked, [&]() {
//        ui->stackedWidget->setCurrentIndex(5);
//    });
//#else
    connect(ui->actionButton, &QPushButton::clicked, [&]() {
        ui->stackedWidget->setCurrentIndex(0);
    });
    connect(ui->commandButton, &QPushButton::clicked, [&]() {
        ui->stackedWidget->setCurrentIndex(1);
    });
    connect(ui->messageButton, &QPushButton::clicked, [&]() {
        ui->stackedWidget->setCurrentIndex(2);
    });
    connect(ui->authButton, &QPushButton::clicked, [&]() {
        ui->stackedWidget->setCurrentIndex(3);
    });
    connect(ui->advancedSettingButton, &QPushButton::clicked, [&]() {
        ui->stackedWidget->setCurrentIndex(4);
    });
//#endif
    //ログアウト
    connect(ui->logoutButton, &QPushButton::clicked, [&]() {
        if (QMessageBox::question(this, tr("確認"), tr("Twitterからログアウトしてアプリケーションを再起動します。\nよろしいですか？"),
                                  QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes) {
            settings->setValue("ConsumerKey", UpdateNameQt::defaultSettings["ConsumerKey"]);
            settings->setValue("ConsumerSecret", UpdateNameQt::defaultSettings["ConsumerSecret"]);
            settings->setValue("AccessToken", UpdateNameQt::defaultSettings["AccessToken"]);
            settings->setValue("AccessTokenSecret", UpdateNameQt::defaultSettings["AccessTokenSecret"]);
            QMessageBox::information(this, tr("ログアウト"), tr("ログアウトしました。\nアプリケーションを再起動します。"), QMessageBox::Ok);
            QApplication::exit(UpdateNameQt::ExitRestart);
        }
    });
    //設定ファイルを開く
    connect(ui->openSettingsFileButton, &QPushButton::clicked, [&]() {
        QDesktopServices::openUrl(QUrl(UpdateNameQt::settings->fileName()));
    });
    //設定ファイルの削除
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
//#ifndef Q_OS_ANDROID
//    //ダイアログのボタン
//    connect(ui->buttonBox, &QDialogButtonBox::clicked, [&](QAbstractButton *button) {
//        switch (ui->buttonBox->standardButton(button)) {
//        case QDialogButtonBox::RestoreDefaults:
//            if (QMessageBox::question(this, tr("確認"), tr("設定を標準設定に戻します。\nよろしいですか？"), QMessageBox::Yes, QMessageBox::No)
//                == QMessageBox::Yes) {
//                //認証設定以外を標準設定をセットする
//                for (auto i = UpdateNameQt::defaultSettings.constBegin(); i != UpdateNameQt::defaultSettings.constEnd(); i++)
//                    if (i.key() != "ConsumerKey" || i.key() != "ConsumerSecret" || i.key() != "AccessToken" || i.key() != "AccessTokenSecret")
//                        settings->setValue(i.key(), i.value());
//                loadSettings();
//            }
//            break;
//        case QDialogButtonBox::Apply:
//            saveSettings();
//            break;
//        case QDialogButtonBox::Cancel:
//            reject();
//            break;
//        case QDialogButtonBox::Ok:
//            saveSettings();
//            accept();
//            break;
//        default:
//            break;
//        }
//    });
//#endif

    loadSettings();
}

PreferencesDialog::~PreferencesDialog()
{
    delete ui;
}

/**
 * @brief 設定ファイルの内容を読み込むメソッド
 */
void PreferencesDialog::loadSettings()
{
    QStringList keys = UpdateNameQt::defaultSettings.keys();
    QCheckBox *checkBox;
    QPlainTextEdit *text;
    QLineEdit *line;

    for (auto i = keys.constBegin(); i != keys.constEnd(); i++) {
        QString keyWord = *i;
        //先頭文字を小文字にする
        keyWord.replace(0, 1, keyWord.at(0).toLower());

        if ((checkBox = findChild<QCheckBox *>(keyWord + "Check")) != NULL)
            checkBox->setChecked(settings->value(*i, UpdateNameQt::defaultSettings[*i]).toBool());
        else if ((text = findChild<QPlainTextEdit *>(keyWord + "Text")) != NULL)
            text->setPlainText(settings->value(*i, UpdateNameQt::defaultSettings[*i]).toString());
        else if ((line = findChild<QLineEdit *>(keyWord + "Line")) != NULL)
            line->setText(settings->value(*i, UpdateNameQt::defaultSettings[*i]).toString());
    }

    ui->over20CharNameButton1->setChecked(settings->value("Over20CharName", UpdateNameQt::defaultSettings["Over20CharName"]).toInt() == UpdateNameQt::ChopName);
    ui->over20CharNameButton2->setChecked(settings->value("Over20CharName", UpdateNameQt::defaultSettings["Over20CharName"]).toInt() == UpdateNameQt::CancelUpdateName);
}

/**
 * @brief 設定ファイルに保存するメソッド
 */
void PreferencesDialog::saveSettings()
{
    auto checkBoxs = findChildren<QCheckBox *>();
    auto groupBoxs = findChildren<QGroupBox *>();
    auto texts = findChildren<QPlainTextEdit *>();
    auto lines = findChildren<QLineEdit *>();

    for (QCheckBox *checkBox : checkBoxs)
        settings->setValue(checkBox->objectName()
                           .replace(0, 1, checkBox->objectName().at(0).toUpper())
                           .remove(QRegExp("Check$")), checkBox->isChecked());
    for (QGroupBox *groupBox : groupBoxs)
        settings->setValue(groupBox->objectName()
                           .replace(0, 1, groupBox->objectName().at(0).toUpper())
                           .remove(QRegExp("Check$")), groupBox->isChecked());
    for (QPlainTextEdit *text : texts)
        settings->setValue(text->objectName()
                           .replace(0, 1, text->objectName().at(0).toUpper())
                           .remove(QRegExp("Text$")), text->toPlainText());
    for (QLineEdit *line : lines)
        settings->setValue(line->objectName()
                           .replace(0, 1, line->objectName().at(0).toUpper())
                           .remove(QRegExp("Line$")), line->text());

    settings->setValue("Over20CharName", ui->over20CharNameButton1->isChecked());
}

//void PreferencesDialog::closeEvent(QCloseEvent *)
//{
//#ifdef Q_OS_ANDROID
//    saveSettings();
//#endif
//}

//void PreferencesDialog::keyReleaseEvent(QKeyEvent *event)
//{
//#ifdef Q_OS_ANDROID
//    if (event->key() == Qt::Key_Back) {
//        if (ui->stackedWidget->currentIndex() == 0)
//            close();
//        else
//            ui->stackedWidget->setCurrentIndex(0);
//        event->accept();
//    }
//#endif
//}
