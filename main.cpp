#include "mainwindow.h"
#include "authdialog.h"
#include "settings.h"
#include <QApplication>
#include <QLockFile>
#include <QDir>
#include <QDebug>

int main(int argc, char *argv[])
{
#ifdef Q_OS_MAC
    QDir dir(argv[0]);
    dir.cdUp();
    dir.cdUp();
    dir.cd("PlugIns");
    QCoreApplication::addLibraryPath(dir.path());
#endif
    QApplication a(argc, argv);
    QLockFile lockfile(".update_name_Qt_lockfile");
    Settings settings;
    int result = 0;

    lockfile.tryLock();
    if(lockfile.error() != QLockFile::NoError) {
        return 1;
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
