#include "updatenameqtglobal.h"
#include "mainwindow.h"
#include "authdialog.h"

#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include <QFile>
#include <QMenuBar>

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
        QMenuBar *bar = new QMenuBar(0);
        QStringList settingsKeys;

        bar->show();

        //日本語化
#ifdef QT_DEBUG
        t.load("qt_ja", QLibraryInfo::location(QLibraryInfo::TranslationsPath));
#else
        t.load("qt_ja", a.applicationDirPath());
#endif
        a.installTranslator(&t);

        //設定ファイルの初期化
        settings = new QSettings(QSettings::IniFormat, QSettings::UserScope, "update_name_Qt_config_2");
        settingsKeys = settings->allKeys();

        //バージョン名のセット
        a.setApplicationVersion("v2.0.1-dev " + QSysInfo::productType());
        //アイコンのセット
        a.setWindowIcon(QIcon(":/icon/icon/icon.png"));
        //常駐できるようにする
        a.setQuitOnLastWindowClosed(false);

        //標準設定のセット
        for (auto i = UpdateNameQt::defaultSettings.begin(); i != UpdateNameQt::defaultSettings.end(); i++) {
            if (settingsKeys.indexOf(i.key()) == -1)
                settings->setValue(i.key(), i.value());
        }
        //認証されていなかったら認証ダイアログを出す
        if (settings->value("AccessToken").toString().isEmpty() || settings->value("AccessTokenSecret").toString().isEmpty()) {
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

        delete bar;

        MainWindow w;
        w.show();

        result = a.exec();
    } while (result == UpdateNameQt::ExitRestart);
    return result;
}
