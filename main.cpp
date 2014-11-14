#include "mainwindow.h"
#include "authdialog.h"
#include "settings.h"
#include <QApplication>
#include <QLockFile>
#include <QStandardPaths>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QLockFile lockfile(QStandardPaths::writableLocation(QStandardPaths::CacheLocation).append("/.~lockfile"));
    Settings settings;

    int result = 0;

    if(lockfile.isLocked()) {
        return 1;
    } else {
        lockfile.lock();
    }

    a.setQuitOnLastWindowClosed(false);
    a.setWindowIcon(QIcon(":/icon/update_name_icon.png"));

    do {
        if(settings.consumerKey().isEmpty()
                || settings.consumerSecret().isEmpty()
                || settings.accessToken().isEmpty()
                || settings.accessTokenSecret().isEmpty()) {
            AuthDialog d;
            if(d.exec() != d.Accepted) {
                return 1;
            }
        }

        MainWindow w;
        w.show();

        result = a.exec();
    } while(result == 255);

    lockfile.unlock();
    return result;
}
