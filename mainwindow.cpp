#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "updatenameqtglobal.h"

#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif

#include <QMessageBox>
#include <QFileDialog>
#include <QStandardPaths>
#include <QFile>

using UpdateNameQt::settings;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{    
    ui->setupUi(this);
#ifdef Q_OS_ANDROID
    ui->saveLogAction->setVisible(false);
#endif

    //connect
    //ログのクリア
    connect(ui->clearLogAction, SIGNAL(triggered()), ui->logText, SLOT(clear()));
    //ログの保存
    connect(ui->saveLogAction, &QAction::triggered, [&]() {
        QFileDialog dialog(this, tr("ログファイルの保存"), QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
        dialog.selectFile("update_name_Qt_log.txt");
        dialog.setAcceptMode(QFileDialog::AcceptSave);

        if (dialog.exec() != QFileDialog::Accepted)
            return;

        QFile file(dialog.selectedFiles().first());
        if (file.open(QFile::WriteOnly | QFile::Text))
            if (file.write(ui->logText->toPlainText().toUtf8())) {
                QMessageBox::information(this, tr("保存しました"), tr("ログファイルを\n%1\nに保存しました。").arg(file.fileName()), QMessageBox::Ok);
                return;
            }
        QMessageBox::critical(this, tr("エラー"), tr("ファイルの保存に失敗しました。\n%1").arg(file.errorString()), QMessageBox::Ok);
    });
    //終了
    connect(ui->quitAction, SIGNAL(triggered()), QApplication::instance(), SLOT(quit()));
    //設定
    connect(ui->preferencesAction, SIGNAL(triggered()), &m_preferencesDialog, SLOT(exec()));

    //Qtについて
    connect(ui->aboutQtAction, &QAction::triggered, [&]() {
        QMessageBox::aboutQt(this, tr("Qtについて"));
    });
    //ログ
    connect(&m_updateName, &UpdateName::started, [&]() {
        QMetaObject::invokeMethod(ui->updateNameSwitch, "setChecked", Qt::QueuedConnection, Q_ARG(bool, true));
        QMetaObject::invokeMethod(ui->logText, "appendPlainText", Qt::QueuedConnection, Q_ARG(QString, tr("update_nameを起動しました。")));
    });
    connect(&m_updateName, &UpdateName::screenNameLookuped, [&](const QString &screenName) {
        QMetaObject::invokeMethod(ui->logText, "appendPlainText", Qt::QueuedConnection,
                                  Q_ARG(QString, tr("screen_nameを取得しました。\nあなたのscreen_nameは@%1です。").arg(screenName)));
    });
    connect(&m_updateName, &UpdateName::running, [&]() {
        QMetaObject::invokeMethod(ui->logText, "appendPlainText", Qt::QueuedConnection, Q_ARG(QString, tr("update_nameを開始しました。")));
    });
    connect(&m_updateName, &UpdateName::stopping, [&]() {
        QMetaObject::invokeMethod(ui->logText, "appendPlainText", Qt::QueuedConnection, Q_ARG(QString, tr("update_nameを終了しています。")));
    });
    connect(&m_updateName, &UpdateName::finished, [&]() {
        QMetaObject::invokeMethod(ui->updateNameSwitch, "setChecked", Qt::QueuedConnection, Q_ARG(bool, false));
        QMetaObject::invokeMethod(ui->logText, "appendPlainText", Qt::QueuedConnection, Q_ARG(QString, tr("update_nameを終了しました。")));
    });
    connect(&m_updateName, &UpdateName::error, [&](UpdateName::State state, const QString &errorMessage) {
        QMetaObject::invokeMethod(ui->logText, "appendPlainText", Qt::QueuedConnection, Q_ARG(QString, tr("%1のエラー: %2").arg(state == UpdateName::UserStream
                                                                                                                            ? "ユーザーストリーム" : "スクリーンネーム取得",
                                                                                                                            errorMessage)));
    });
    connect(&m_updateName, &UpdateName::updateStarted, [&](UpdateProfile::UpdateType type, const TwitterAPI::Object::Users &executedUser) {
        QMetaObject::invokeMethod(ui->logText, "appendPlainText", Qt::QueuedConnection, Q_ARG(QString, tr("\"%1@%2\"がupdate_%3を実行しました")
                                                                                              .arg(executedUser.name(), executedUser.screenName(), updateTypeToString(type))));
    });
    connect(&m_updateName, &UpdateName::updateFinished, [&](UpdateProfile::UpdateType type, const QString &newProfile) {
        QMetaObject::invokeMethod(ui->logText, "appendPlainText", Qt::QueuedConnection, Q_ARG(QString, tr("%1を\"%2\"に変更しました。").arg(updateTypeToString(type), newProfile)));
    });
    connect(&m_updateName, &UpdateName::resultPosted, [&]() {
        QMetaObject::invokeMethod(ui->logText, "appendPlainText", Qt::QueuedConnection, Q_ARG(QString, tr("結果をツイートしました。")));
    });
    connect(&m_updateName, &UpdateName::updateError, [&](UpdateProfile::UpdateType type, const QString &errorMessage) {
        QMetaObject::invokeMethod(ui->logText, "appendPlainText", Qt::QueuedConnection, Q_ARG(QString, tr("update_%1に失敗しました。: %2").arg(updateTypeToString(type), errorMessage)));
    });
    connect(&m_updateName, &UpdateName::resultPostError, [&](const QString &errorMessage) {
        QMetaObject::invokeMethod(ui->logText, "appendPlainText", Qt::QueuedConnection, Q_ARG(QString, tr("結果のツイートに失敗しました。: %1").arg(errorMessage)));
    });

    connect(ui->updateNameSwitch, &QPushButton::clicked, [&]() {
        if (ui->updateNameSwitch->isChecked()) {
            ui->updateNameSwitch->setChecked(false);
            m_updateName.start();
        } else {
            ui->updateNameSwitch->setChecked(true);
            m_updateName.stop();
        }
    });
    connect(ui->updateNameSwitch, &QPushButton::toggled, [&](bool checked) {
            ui->updateNameSwitch->setText(tr(checked ? "update_nameを終了する" : "update_nameを開始する"));
    });
}

MainWindow::~MainWindow()
{
    m_updateName.stop();
    m_updateName.wait();
    delete ui;
}

QString MainWindow::updateTypeToString(UpdateProfile::UpdateType type)
{
    switch (type) {
    case UpdateProfile::Name:
        return "name";
    case UpdateProfile::Url:
        return "url";
    case UpdateProfile::Location:
        return "location";
    case UpdateProfile::Description:
        return "description";
    case UpdateProfile::Image:
        return "image";
    case UpdateProfile::Background:
        return "background";
    case UpdateProfile::Banner:
        return "banner";
    }
    return NULL;
}
