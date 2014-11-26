#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settings.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

#ifdef Q_OS_WIN
    //Windows 10だとsizeHint()が機能しない
    setFixedSize(size());
#else
    setFixedSize(sizeHint());
#endif


    //システムトレイのセットアップ
    m_systemTray                 = new QSystemTrayIcon(QIcon(":/icon/update_name_icon.png"), this);
    m_systemTrayMenu             = new QMenu(this);
    m_systemTrayActionQuit       = new QAction(tr("update_name_Qtを終了する(&Q)"), m_systemTrayMenu);
    m_systemTrayActionShowWindow = new QAction(tr("メインウィンドウを表示(&S)"), m_systemTrayMenu);

    m_systemTrayMenu->addActions(QList<QAction *>()
                                  << m_systemTrayActionShowWindow
                                  << m_systemTrayActionQuit);
    m_systemTray->setContextMenu(m_systemTrayMenu);
    m_systemTray->show();

    //ウェルカムメッセージの表示
    QMetaObject::invokeMethod(this, "writeWelcomeLog", Qt::QueuedConnection);
    ui->statusBar->showMessage(tr("update_name_Qtへようこそ"));

    //connect
    connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(quitUpdateNameQt()));
    connect(ui->actionAbout, &QAction::triggered, [&]() {
        QMessageBox::about(this, tr("update_name_Qtについて"),
                           tr("<h1><b>update_name_Qt</b></h1><br />"
                              "オープンソースでクロスプラットフォームなupdate_nameのGUIフロントエンド<br />"
                              "<br />(c) 2014 owata_programer<br />"
                              "<br />"
                              "Twitter -&gt; </span><a href=\"https://twitter.com/owata_programer\"><span style=\"text-decoration: underline; color:#0000ff;\">"
                              "@owata_programer</span></a></p><p>ウェブサイト -&gt; <a href=\"https://sites.google.com/site/owataprogramersite/\"><span style=\"text-decoration: underline; color:#0000ff;\">https://sites.google.com/site/owataprogramersite/</span></a>"));
    });
    connect(ui->actionPreferences, SIGNAL(triggered()), &m_preferencesDialog, SLOT(exec()));
    connect(m_systemTrayActionQuit, SIGNAL(triggered()), this, SLOT(quitUpdateNameQt()));
    connect(m_systemTrayActionShowWindow, &QAction::triggered, [&]() {
        showNormal();
        activateWindow();
        raise();
        setFocus();
    });
    connect(m_systemTray, &QSystemTrayIcon::activated, [&](QSystemTrayIcon::ActivationReason reason) {
        if(reason == QSystemTrayIcon::DoubleClick) {
            m_systemTrayActionShowWindow->trigger();
        }
    });
    connect(ui->updateNameSwitchButton, &QPushButton::clicked, [&]() {
        static bool started = m_settings.isAutoStartUpdateName();
        if(started) {
            stopUpdateName();
        } else {
            startUpdateName();
        }
        started = !started;
    });
    connect(ui->actionSaveLog, SIGNAL(triggered()), this, SLOT(saveLog()));
    connect(ui->actionUpdateNameSender, SIGNAL(triggered()), &m_updateNameSender, SLOT(exec()));
    connect(ui->actionAboutQt, &QAction::triggered, [&]() {
        QMessageBox::aboutQt(this, tr("Qtについて"));
    });
    connect(ui->actionSwitchUpdateName, SIGNAL(triggered()), ui->updateNameSwitchButton, SIGNAL(clicked()));

    qRegisterMetaType<UserStream::State>("UserStream::State");
    connect(&m_userStream, SIGNAL(stateChanged(UserStream::State)), this, SLOT(writeUserStreamLog(UserStream::State)));
    connect(&m_userStream, SIGNAL(receivedData(QByteArray)), &m_updateProfile, SLOT(exec(QByteArray)));

    connect(&m_updateProfile, &UpdateProfile::executed, [&](const UpdateProfile::ProfileType &type, const UsersObject &executedUser) {
        QMetaObject::invokeMethod(this, "writeLog", Qt::QueuedConnection,
                                  Q_ARG(QString, tr("\"@%1\"によってupdate_%2が実行されました。")
                                        .arg(executedUser.screen_name(), UpdateProfile::profileTypeString(type))));
        QMetaObject::invokeMethod(m_systemTray, "showMessage", Qt::QueuedConnection,
                                  Q_ARG(QString, tr("update_%1が実行されました。").arg(UpdateProfile::profileTypeString(type))),
                                  Q_ARG(QString, tr("\"@%1\"によってupdate_%2が実行されました。")
                                        .arg(executedUser.screen_name(), UpdateProfile::profileTypeString(type))));
    });
    connect(&m_updateProfile, &UpdateProfile::updated, [&](const UpdateProfile::ProfileType &type, const QString &updatedProfileValue) {
        QMetaObject::invokeMethod(this, "writeLog", Qt::QueuedConnection,
                                  Q_ARG(QString, tr("%1が\"%2\"に変更されました。")
                                        .arg(UpdateProfile::profileTypeString(type), updatedProfileValue)));
        QMetaObject::invokeMethod(m_systemTray, "showMessage", Qt::QueuedConnection,
                                  Q_ARG(QString, tr("%1が変更されました。").arg(UpdateProfile::profileTypeString(type))),
                                  Q_ARG(QString, tr("%1が\"%2\"に変更されました。")
                                        .arg(UpdateProfile::profileTypeString(type), updatedProfileValue)));
    });
    connect(&m_updateProfile, &UpdateProfile::resultRecieved, [&]() {
        QMetaObject::invokeMethod(this, "writeLog", Qt::QueuedConnection, Q_ARG(QString, tr("結果をツイートしました。")));
    });
    connect(&m_updateProfile, &UpdateProfile::updateError,
            [&](const UpdateProfile::ProfileType &type, const Update::ErrorState &state, const QString &errorMessage) {
        QString log;
        switch(state) {
        case Update::UpdateFailed:
            log = tr("%1の変更に失敗しました。: %2").arg(UpdateProfile::profileTypeString(type), errorMessage);
            break;
        case Update::ResultRecieveFailed:
            log = tr("結果のツイートに失敗しました。: %1").arg(errorMessage);
            break;
        }
        QMetaObject::invokeMethod(this, "writeLog", Qt::QueuedConnection, Q_ARG(QString, log));
    });
    connect(&m_updateProfile, &UpdateProfile::stateChanged, [&](const UpdateProfile::State &state) {
        QString log;
        switch(state) {
        case UpdateProfile::GetScreenNameFinished:
            log = tr("screen_nameを取得しました\n"
                     "あなたのscreen_name: %1").arg(m_updateProfile.screenName());
            break;
        }
        QMetaObject::invokeMethod(this, "writeLog", Qt::QueuedConnection, Q_ARG(QString, log));
    });
    connect(&m_updateProfile, &UpdateProfile::error, [&](const UpdateProfile::ErrorState &state) {
        QString log;
        switch(state) {
        case UpdateProfile::GetScreenNameFailed:
            log = tr("screen_nameの取得に失敗しました。: %1").arg(m_updateProfile.errorString());
            break;
        }
        QMetaObject::invokeMethod(this, "writeLog", Qt::QueuedConnection, Q_ARG(QString, log));
    });

    m_updateProfile.moveToThread(m_updateProfileThread);
    m_updateProfileThread->start();

    if(m_settings.isAutoStartUpdateName()) {
        startUpdateName();
    }
}

MainWindow::~MainWindow()
{
    stopUpdateName();
    m_updateProfileThread->quit();
    m_systemTray->deleteLater();
    m_systemTrayMenu->deleteLater();
    m_systemTrayActionQuit->deleteLater();
    m_systemTrayActionShowWindow->deleteLater();
    delete ui;
    if(m_userStream.isRunning()) {
        m_userStream.wait();
    }
    if(m_updateProfileThread->isRunning()) {
        m_updateProfileThread->wait();
    }
    m_updateProfileThread->deleteLater();
}

void MainWindow::writeLog(const QString &log)
{
    ui->logConsole->appendPlainText(log);
}

void MainWindow::writeWelcomeLog()
{
    writeLog(tr("update_name_Qtへようこそ。\n"
                "ボタンをクリックするだけでupdate_nameが誰にでも簡単に使えます。\n"));
}

void MainWindow::writeUserStreamLog(UserStream::State state)
{
    switch(state) {
    case UserStream::ConnectionFailed:
        writeLog(tr("UserStreamへの接続に失敗しました。"));
        break;
    case UserStream::Connecting:
        writeLog(tr("UserStreamに接続しています..."));
        break;
    case UserStream::Running:
        writeLog(tr("UserStreamに接続しました。"));
        break;
    case UserStream::DisConnected:
        writeLog(tr("UserStreamから切断されました。"));
        break;
    case UserStream::Waiting:
        writeLog(tr("待機しています..."));
        break;
    }
}

void MainWindow::startUpdateName()
{
    if(m_userStream.isRunning()) {
        return;
    }

    m_userStream.start();
    if(m_settings.isPostStartupMessage()) {
        try {
            m_twitter.statusUpdate(m_settings.startupMessage());
            writeLog(tr("スタートアップメッセージをツイートしました。"));
        } catch(const std::runtime_error &e) {
            writeLog(tr("スタートアップメッセージのツイートに失敗しました。: %1").arg(QString::fromStdString(e.what())));
        }
    }

    ui->updateNameSwitchButton->setText(tr("update_nameストップ"));
    ui->actionSwitchUpdateName->setText(tr("update_nameを停止する(&U)"));
    ui->statusBar->showMessage(tr("update_nameを開始しました。"));
}

void MainWindow::stopUpdateName()
{
    if(m_userStream.isFinished()) {
        return;
    }

    m_userStream.stop();
    if(m_settings.isPostClosedMessage()) {
        try {
            m_twitter.statusUpdate(m_settings.closedMessage());
            writeLog(tr("終了メッセージをツイートしました。"));
        } catch(const std::runtime_error &e) {
            writeLog(tr("終了メッセージのツイートに失敗しました。: %1").arg(QString::fromStdString(e.what())));
        }
    }

    ui->updateNameSwitchButton->setText(tr("update_nameスタート"));
    ui->actionSwitchUpdateName->setText(tr("update_nameを開始する(&U)"));
    ui->statusBar->showMessage(tr("update_nameを停止しました。"));
}

void MainWindow::closeEvent(QCloseEvent *)
{
    if(m_settings.isStayOnSystemTray()) {
        m_systemTray->showMessage(tr("update_name_Qtは動作してます"),
                                 tr("update_name_Qtは動作中です。\n"
                                    "終了させるにはアイコンを右クリックして、「update_name_Qtを終了する」をクリックしてください。"));
    } else {
        QApplication::quit();
    }
}

void MainWindow::quitUpdateNameQt()
{
    //メインウィンドウを表示
    showNormal();
    activateWindow();
    raise();
    setFocus();

    //質問ダイアログの表示
    if(QMessageBox::question(this,
                          tr("確認"),
                          tr("update_name_qtを終了します。\n"
                             "よろしいですか？"),
                          QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes) {
        QApplication::quit();
    }
}

void MainWindow::saveLog()
{
    QFileDialog dialog(this, tr("ログの保存"), QDir::homePath());
    QFile logfile;

    dialog.selectFile("update_name_log.txt");
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    if(dialog.exec() != QFileDialog::Accepted) {
        return;
    }

    logfile.setFileName(dialog.selectedFiles().first());
    if(logfile.open(QFile::WriteOnly)) {
        if(logfile.write(ui->logConsole->toPlainText().toUtf8())) {
            logfile.close();
            QMessageBox::information(this,
                              tr("保存しました"),
                              tr("ログの保存が完了しました。"),
                              QMessageBox::Ok);
            return;
        }
    }
    QMessageBox::critical(this,
                          tr("エラー"),
                          tr("ログの保存に失敗しました。\n%1").arg(logfile.errorString()),
                          QMessageBox::Ok);
}
