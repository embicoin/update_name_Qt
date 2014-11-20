#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settings.h"

#include <QMessageBox>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

#ifdef Q_OS_WIN
    setFixedSize(size());
#else
    setFixedSize(sizeHint());
#endif

    system_tray = new QSystemTrayIcon(QIcon(":/icon/update_name_icon.png"), this);
    system_tray_menu = new QMenu(this);
    system_tray_action_quit = new QAction(tr("update_name_Qtを終了する(&Q)"), system_tray_menu);
    system_tray_action_show_window = new QAction(tr("メインウィンドウを表示(&S)"), system_tray_menu);
    system_tray_menu->addActions(QList<QAction *>() << system_tray_action_show_window << system_tray_action_quit);
    system_tray->setContextMenu(system_tray_menu);
    system_tray->show();

    update_profile.moveToThread(update_name_thread);

    //about_dialog.setParent(this, Qt::Dialog | Qt::FramelessWindowHint);
    QMetaObject::invokeMethod(this, "writeWelcomeLog", Qt::QueuedConnection);
    ui->statusBar->showMessage(tr("update_name_Qtへようこそ"));

    connect(ui->actionQuit, &QAction::triggered, [&]() {
        showNormal();
        activateWindow();
        raise();
        setFocus();
        if(QMessageBox::question(this,
                              tr("確認"),
                              tr("update_name_qtを終了します。\n"
                                 "よろしいですか？"),
                              QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes) {
            QApplication::quit();
        }
    });
    connect(ui->actionAbout, &QAction::triggered, [&]() {
        QMessageBox::about(this, tr("update_name_Qtについて"),
                           tr("<h1><b>update_name_Qt</b></h1><br />"
                              "オープンソースでクロスプラットフォームなupdate_nameのGUIフロントエンド<br />"
                              "<br />(c) 2014 owata_programer<br />"
                              "<br />"
                              "Twitter -&gt; </span><a href=\"https://twitter.com/owata_programer\"><span style=\"text-decoration: underline; color:#0000ff;\">"
                              "@owata_programer</span></a></p><p>ウェブサイト -&gt; <a href=\"https://sites.google.com/site/owataprogramersite/\"><span style=\"text-decoration: underline; color:#0000ff;\">https://sites.google.com/site/owataprogramersite/</span></a>"));
    });
    connect(ui->actionPreferences, SIGNAL(triggered()), &preferences_dialog, SLOT(exec()));
    connect(system_tray_action_quit, SIGNAL(triggered()), ui->actionQuit, SIGNAL(triggered()));
    connect(system_tray_action_show_window, &QAction::triggered, [&]() {
        showNormal();
        activateWindow();
        raise();
        setFocus();
    });
    connect(system_tray, &QSystemTrayIcon::activated, [&](QSystemTrayIcon::ActivationReason reason) {
        if(reason == QSystemTrayIcon::DoubleClick) {
            system_tray_action_show_window->trigger();
        }
    });
    connect(ui->updateNameSwitchButton, &QPushButton::clicked, [&]() {
        static bool started = settings.isAutoStartUpdateName();
        if(started) {
            stopUpdateName();
        } else {
            startUpdateName();
        }
        started = !started;
    });
    connect(ui->actionSaveLog, &QAction::triggered, [&]() {
        QFileDialog dialog(this,
                           tr("ログの保存"),
                           QDir::homePath());
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
    });
    connect(ui->actionUpdateNameSender, SIGNAL(triggered()), &update_name_sender, SLOT(exec()));
    connect(ui->actionAboutQt, &QAction::triggered, [&]() {
        QMessageBox::aboutQt(this, tr("Qtについて"));
    });
    connect(ui->actionSwitchUpdateName, SIGNAL(triggered()), ui->updateNameSwitchButton, SIGNAL(clicked()));

    qRegisterMetaType<UserStream::State>("UserStream::State");
    connect(&user_stream, SIGNAL(stateChanged(UserStream::State)), this, SLOT(writeUserStreamLog(UserStream::State)));
    connect(&user_stream, SIGNAL(receivedData(QByteArray)), &update_profile, SLOT(exec(QByteArray)));
    qRegisterMetaType<Update::State>("Update::State");
    qRegisterMetaType<UpdateProfile::ProfileType>("UpdateProfile::ProfileType");
    connect(&update_profile, SIGNAL(stateChanged(Update::State,UpdateProfile::ProfileType)), this, SLOT(writeUpdateProfileLog(Update::State,UpdateProfile::ProfileType)));

    if(settings.isAutoStartUpdateName()) {
        startUpdateName();
    }
}

MainWindow::~MainWindow()
{
    stopUpdateName();
    system_tray->deleteLater();
    system_tray_menu->deleteLater();
    system_tray_action_quit->deleteLater();
    system_tray_action_show_window->deleteLater();
    update_name_thread->deleteLater();
    delete ui;
    if(user_stream.isRunning()) {
        user_stream.wait();
    }
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

void MainWindow::writeUpdateProfileLog(Update::State state, UpdateProfile::ProfileType type)
{
    QString profile_type_string;
    const QString updated_profile = update_profile.profileValue();
    const QString error_string = update_profile.errorString();
    switch (type) {
        case UpdateProfile::Name:
            profile_type_string = tr("name");
            break;
        case UpdateProfile::Url:
            profile_type_string = tr("url");
            break;
        case UpdateProfile::Location:
            profile_type_string = tr("location");
            break;
        case UpdateProfile::Description:
            profile_type_string = tr("description");
            break;
    }

    switch (state) {
    case Update::Aborted:
        writeLog(tr("udpate_%1が拒否されました。").arg(profile_type_string));
        break;
    case Update::Executed:
        writeLog(tr("\"@%1\"によってupdate_%2が実行されました。").arg(update_profile.executedUserScreenName(), profile_type_string));
        system_tray->showMessage(tr("update_%1が実行されました。").arg(profile_type_string),
                                 tr("update_%1によってupdate_%2が実行されました。").arg(update_profile.executedUserScreenName(), profile_type_string));
        break;
    case Update::UpdateSuccessed:
        writeLog(tr("%1が\"%2\"に変更されました。").arg(profile_type_string, updated_profile));
        system_tray->showMessage(tr("%1が変更されました").arg(profile_type_string),
                                 tr("%1が\"%2\"に変更されました。").arg(profile_type_string, updated_profile));
        break;
    case Update::UpdateFailed:
        writeLog(tr("%1の変更に失敗しました。: %2").arg(profile_type_string, error_string));
        system_tray->showMessage(tr("%1の変更に失敗しました").arg(profile_type_string),
                                 error_string, QSystemTrayIcon::Warning);
        break;
    case Update::RecieveResultSuccessed:
        writeLog(tr("結果をツイートしました。"));
        break;
    case Update::RecieveResultFailed:
        writeLog(tr("結果のツイートに失敗しました。: %1").arg(error_string));
        break;
    }
}

void MainWindow::startUpdateName()
{
    if(user_stream.isRunning()) {
        return;
    }

    user_stream.start();
    update_name_thread->start();
    if(settings.isPostStartupMessage()) {
        try {
            update_profile.postStartupMessage();
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
    if(user_stream.isFinished()) {
        return;
    }

    user_stream.stop();
    update_name_thread->quit();
    if(settings.isPostClosedMessage()) {
        try {
            update_profile.postClosedMessage();
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
    if(settings.isStayOnSystemTray()) {
        system_tray->showMessage(tr("update_name_Qtは動作してます"),
                                 tr("update_name_Qtは動作中です。\n"
                                    "終了させるにはアイコンを右クリックして、「update_name_Qtを終了する」をクリックしてください。"));
    } else {
        QApplication::quit();
    }
}
