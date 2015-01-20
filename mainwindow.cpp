#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "updatenameqtglobal.h"

#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif

#include <QMessageBox>
#include <QFileDialog>
#include <QStandardPaths>
#include <QDesktopServices>
#include <QFile>

using UpdateNameQt::settings;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //ウィンドウタイトルの設定
    setWindowTitle(QApplication::applicationName() + " " + QApplication::applicationVersion());
    //メニューバーのセットアップ
    m_menuBar                       = new QMenuBar(0);
    auto *fileMenu                  = new QMenu(tr("ファイル(&F)"), m_menuBar);
    auto *toolsMenu                 = new QMenu(tr("ツール(&T)"), m_menuBar);
    auto *helpMenu                  = new QMenu(tr("ヘルプ(&H)"), m_menuBar);
    auto *saveLogAction             = new QAction(tr("ログを保存する"), fileMenu);
    auto *clearLogAction            = new QAction(tr("ログを削除する"), fileMenu);
    auto *quitAction                = new QAction(tr("終了(&Q)"), fileMenu);
    auto *preferencesAction         = new QAction(tr("設定(&P)"), toolsMenu);
    auto *updateNameSenderAction    = new QAction(tr("update_name送信機"), toolsMenu);
    auto *aboutAction               = new QAction(tr("update_name_Qtについて"), helpMenu);
    auto *aboutQtAction             = new QAction(tr("Qtについて"), helpMenu);
    auto *updateNameQtWebSiteAction = new QAction(tr("update_name_Qtのウェブサイト"), helpMenu);

    //アイコンのセットアップ
#ifndef Q_OS_MAC
    saveLogAction->setIcon(QApplication::style()->standardIcon(QStyle::SP_DialogSaveButton));
    clearLogAction->setIcon(QApplication::style()->standardIcon(QStyle::SP_LineEditClearButton));
    quitAction->setIcon(QApplication::style()->standardIcon(QStyle::SP_DialogCloseButton));
    aboutAction->setIcon(QApplication::style()->standardIcon(QStyle::SP_MessageBoxInformation));
#endif
    //メニューロールの設定
    quitAction->setMenuRole(QAction::QuitRole);
    preferencesAction->setMenuRole(QAction::PreferencesRole);
    aboutAction->setMenuRole(QAction::AboutRole);
    aboutQtAction->setMenuRole(QAction::AboutQtRole);
    //メニューのセットアップ
    fileMenu->addAction(saveLogAction);
    fileMenu->addAction(clearLogAction);
    fileMenu->addSeparator();
    fileMenu->addAction(quitAction);
    toolsMenu->addAction(preferencesAction);
    toolsMenu->addAction(updateNameSenderAction);
    helpMenu->addAction(aboutAction);
    helpMenu->addAction(aboutQtAction);
    helpMenu->addAction(updateNameQtWebSiteAction);
    m_menuBar->addMenu(fileMenu);
    m_menuBar->addMenu(toolsMenu);
    m_menuBar->addMenu(helpMenu);

    setMenuBar(m_menuBar);

    //システムトレイのセットアップ
    m_systemTrayIcon           = new QSystemTrayIcon(qApp->windowIcon(), this);
    auto *iconMenu             = new QMenu(this);
    auto *showMainWindowAction = new QAction(tr("メインウィンドウを表示"), iconMenu);

    iconMenu->addActions(QList<QAction *>() << showMainWindowAction << quitAction);
    m_systemTrayIcon->setContextMenu(iconMenu);
    m_systemTrayIcon->show();

    //connect
    //メニュー
    connect(saveLogAction, SIGNAL(triggered()), this, SLOT(saveLog()));
    connect(clearLogAction, SIGNAL(triggered()), ui->logText, SLOT(clear()));
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(preferencesAction, SIGNAL(triggered()), &m_preferencesDialog, SLOT(exec()));
    connect(updateNameSenderAction, SIGNAL(triggered()), &m_updateNameSender, SLOT(exec()));
    connect(aboutAction, SIGNAL(triggered()), &m_aboutDialog, SLOT(exec()));
    connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(updateNameQtWebSiteAction, &QAction::triggered, [&]() {
        QDesktopServices::openUrl(QUrl("http://owataprogramer.orz.hm/works/update_name_Qt.html"));
    });
    //アイコン
    connect(showMainWindowAction, &QAction::triggered, [&]() {
        setWindowState(Qt::WindowActive);
    });

    //Qtについて
    connect(ui->aboutQtAction, &QAction::triggered, [&]() {
        QMessageBox::aboutQt(this, tr("Qtについて"));
    });
    //ログ
    connect(&m_updateName, &UpdateName::started, [&]() {
        QMetaObject::invokeMethod(ui->updateNameSwitch, "setChecked", Qt::QueuedConnection, Q_ARG(bool, true));
        QMetaObject::invokeMethod(ui->logText, "appendPlainText", Qt::QueuedConnection, Q_ARG(QString, tr("update_nameを開始しています…")));
    });
    connect(&m_updateName, &UpdateName::screenNameLookuped, [&](const QString &screenName) {
        QMetaObject::invokeMethod(ui->logText, "appendPlainText", Qt::QueuedConnection,
                                  Q_ARG(QString, tr("screen_nameを取得しました。\nあなたのscreen_nameは@%1です。").arg(screenName)));
    });
    connect(&m_updateName, &UpdateName::running, [&]() {
        QMetaObject::invokeMethod(ui->logText, "appendPlainText", Qt::QueuedConnection, Q_ARG(QString, tr("update_nameを開始しました。")));
        QMetaObject::invokeMethod(m_systemTrayIcon, "showMessage", Qt::QueuedConnection,
                                  Q_ARG(QString, QApplication::applicationName()),Q_ARG(QString, tr("update_nameを開始しました。")));
    });
    connect(&m_updateName, &UpdateName::stopping, [&]() {
        QMetaObject::invokeMethod(ui->logText, "appendPlainText", Qt::QueuedConnection, Q_ARG(QString, tr("update_nameを終了しています…")));
    });
    connect(&m_updateName, &UpdateName::disConnected, [&]() {
        QMetaObject::invokeMethod(ui->logText, "appendPlainText", Qt::QueuedConnection, Q_ARG(QString, tr("UserStreamから切断されました。")));
    });
    connect(&m_updateName, &UpdateName::waitting, [&](uint waitCount) {
        QMetaObject::invokeMethod(ui->logText, "appendPlainText", Qt::QueuedConnection, Q_ARG(QString, tr("%1秒待機します。").arg(waitCount)));
    });
    connect(&m_updateName, &UpdateName::finished, [&]() {
        QMetaObject::invokeMethod(ui->updateNameSwitch, "setChecked", Qt::QueuedConnection, Q_ARG(bool, false));
        QMetaObject::invokeMethod(ui->logText, "appendPlainText", Qt::QueuedConnection, Q_ARG(QString, tr("update_nameを終了しました。")));
        QMetaObject::invokeMethod(m_systemTrayIcon, "showMessage", Qt::QueuedConnection,
                                  Q_ARG(QString, QApplication::applicationName()),Q_ARG(QString, tr("update_nameを終了しました。")));
    });
    connect(&m_updateName, &UpdateName::error, [&](UpdateName::State state, const QString &errorMessage) {
        QMetaObject::invokeMethod(ui->logText, "appendPlainText", Qt::QueuedConnection, Q_ARG(QString, tr("%1のエラー: %2").arg(state == UpdateName::UserStream
                                                                                                                            ? "ユーザーストリーム" : "スクリーンネーム取得",
                                                                                                                            errorMessage)));
    });
    connect(&m_updateName, &UpdateName::updateStarted, [&](UpdateProfile::UpdateType type, const TwitterAPI::Object::Users &executedUser) {
        QMetaObject::invokeMethod(ui->logText, "appendPlainText", Qt::QueuedConnection, Q_ARG(QString, tr("\"%1@%2\"がupdate_%3を実行しました")
                                                                                              .arg(executedUser.name(), executedUser.screenName(), updateTypeToString(type))));
        QMetaObject::invokeMethod(m_systemTrayIcon, "showMessage", Qt::QueuedConnection,
                                  Q_ARG(QString, QApplication::applicationName()),Q_ARG(QString, tr("\"%1@%2\"がupdate_%3を実行しました")
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

    //update_nameスイッチ
    connect(ui->updateNameSwitch, &QPushButton::clicked, [&]() {
        if (ui->updateNameSwitch->isChecked()) {
            ui->updateNameSwitch->setChecked(false);
            m_updateName.start(QThread::HighPriority);
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
    delete m_menuBar;
}

void MainWindow::saveLog()
{
    QString saveFilePath = QFileDialog::getSaveFileName(this, tr("保存"),
                                                        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
    if (saveFilePath.isEmpty())
        return;

    QFile saveFile;
    if (saveFile.open(QFile::WriteOnly | QFile::Text))
        if (saveFile.write(ui->logText->toPlainText().toUtf8())) {
            QMessageBox::information(this, tr("保存"), tr("%1\nに保存しました。").arg(saveFilePath), QMessageBox::Ok);
            return;
        }
    QMessageBox::critical(this, tr("エラー"), tr("保存に失敗しました。\n%1").arg(saveFile.errorString()), QMessageBox::Ok);
}

void MainWindow::closeEvent(QCloseEvent *)
{
    if (!settings->value("StaysOnSystemTray").toBool())
        QApplication::quit();
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
