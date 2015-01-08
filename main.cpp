#include "updatenameqtglobal.h"
#include "mainwindow.h"
//#ifdef Q_OS_ANDROID
//#include <QtAndroidExtras>
//#include "android/authdialog.h"
//#else
#include "authdialog.h"
//#endif

#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include <QFile>
#include <QMenuBar>
#include <QMessageBox>

namespace UpdateNameQt {
QSettings *settings;
}

int main(int argc, char *argv[])
{
//#ifdef Q_OS_ANDROID
//    QAndroidJniObject serviceManager("org/owata_programer/update_name_Qt/ServiceManager");
//#endif
    using UpdateNameQt::settings;
    int result;

    do {
        QApplication a(argc, argv);
        QTranslator t;
        QMenuBar *bar = new QMenuBar(0);

        bar->show();

        //日本語化
        t.load("qt_ja", QLibraryInfo::location(QLibraryInfo::TranslationsPath));
        a.installTranslator(&t);

        //設定ファイルの初期化
        settings = new QSettings(QSettings::IniFormat, QSettings::UserScope, "update_name_Qt_config_2");

        //バージョン名のセット
        QString os;
#ifdef Q_OS_LINUX
//#ifdef Q_OS_ANDROID
//        os = "Android";
//#else
        os = "Linux";
//#endif
#elif defined(Q_OS_MAC)
        os = "MacOS X";
#elif defined(Q_OS_WIN)
        os = "Windows";
#endif
        a.setApplicationVersion("update_name_Qt v2.0.1-dev " + os);

        //標準設定のセット
        if (!QFile::exists(settings->fileName()))
            for (auto i = UpdateNameQt::defaultSettings.begin(); i != UpdateNameQt::defaultSettings.end(); i++)
                settings->setValue(i.key(), i.value());

        //認証されていなかったら認証ダイアログを出す
        if (settings->value("AccessToken").toString().isEmpty() || settings->value("AccessTokenSecret").toString().isEmpty()) {
            bool retry;
            do {
                AuthDialog auth;
                auth.setModal(true);
                retry = false;
                switch (auth.exec()) {
                case AuthDialog::AuthSuccessed:
                    break;
                case AuthDialog::Cancelled:
                    return 1;
                case AuthDialog::Retry:
                    retry = true;
                default:
                    return 1;
                }
                result = UpdateNameQt::ExitRestart;
                continue;
            } while (retry);
        }

        delete bar;

        MainWindow w;
        w.show();

//#ifdef Q_OS_ANDROID
//        //サービス開始
//        serviceManager.callMethod<void>("start");
//#endif

        result = a.exec();

    } while (result == UpdateNameQt::ExitRestart);

//#ifdef Q_OS_ANDROID
//    //サービス停止
//    serviceManager.callMethod<void>("stop");
//#endif

    return result;
}
