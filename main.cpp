#include "updatenameqtglobal.h"
#include "mainwindow.h"
#ifdef Q_OS_ANDROID
#include "android/authdialog.h"
#else
#include "authdialog.h"
#endif

#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>

namespace UpdateNameQt {
QSettings *settings;
}

int main(int argc, char *argv[])
{
    using UpdateNameQt::settings;
    int result;

    do {
        QApplication a(argc, argv);
        QTranslator t;

        //日本語化
        t.load("qt_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
        a.installTranslator(&t);

        //設定ファイルの初期化
        settings = new QSettings(QSettings::IniFormat, QSettings::UserScope, "update_name_Qt_config_2");

        //バージョン名のセット
        QString os;
#ifdef Q_OS_LINUX
#ifdef Q_OS_ANDROID
        os = "Android";
#else
        os = "Linux";
#endif
#elif defined(Q_OS_MAC)
        os = "MacOS X";
#elif defined(Q_OS_WIN)
        os = "Windows";
#endif
        a.setApplicationVersion("update_name_Qt v2.0.1-dev " + os);

        //認証されていなかったら認証ダイアログを出す
        if (settings->value("AccessToken").isNull() || settings->value("AccessTokenSecret").isNull()) {
            bool retry;
            do {
                AuthDialog auth;
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
            } while (retry);
        }

        MainWindow w;
        w.show();

        result = a.exec();

    } while ( result == UpdateNameQt::ExitRestart);

    return result;
}
