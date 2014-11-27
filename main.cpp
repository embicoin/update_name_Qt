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
    qDebug() << "Set library path...";
    QDir dir(argv[0]);
    dir.cdUp();
    dir.cdUp();
    dir.cd("PlugIns");
    QCoreApplication::addLibraryPath(dir.path());
    qDebug() << "Set library path finished\n"
                "LibraryPath:" << QCoreApplication::libraryPaths();
#endif
    int result = 255;

    while(result == 255) {
        QApplication a(argc, argv);
        QLockFile lockfile(".update_name_Qt_lockfile");
        Settings settings;

        lockfile.tryLock();
        if(lockfile.error() != QLockFile::NoError) {
            qWarning() << "update_name_Qt is already running!";
            return 1;
        }

        a.setQuitOnLastWindowClosed(false);
        a.setWindowIcon(QIcon(":/icon/update_name_icon.png"));

        if(settings.consumerKey().isEmpty()
                || settings.consumerSecret().isEmpty()
                || settings.accessToken().isEmpty()
                || settings.accessTokenSecret().isEmpty()) {
            AuthDialog d;
            switch (d.exec()) {
            case QDialog::Rejected:
                return 1;
            case 255:
                continue;
            default:
                break;
            }
        }

        MainWindow w;
        w.show();

        result = a.exec();

        lockfile.unlock();

    }

    return result;
}
