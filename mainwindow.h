#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//#include "aboutdialog.h"
#include "preferencesdialog.h"
#include "userstream.h"
#include "updateprofile.h"
#include "updatenamesender.h"

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QCloseEvent>
#include <QThread>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    virtual void closeEvent(QCloseEvent *);

private slots:
    void writeLog(const QString &log);
    void writeWelcomeLog();
    void writeUserStreamLog(UserStream::State state);
    void writeUpdateProfileLog(Update::State state, UpdateProfile::ProfileType type);
    void startUpdateName();
    void stopUpdateName();

private:
    Ui::MainWindow *ui;
    QSystemTrayIcon *system_tray;
    QMenu *system_tray_menu;
    QAction *system_tray_action_quit;
    QAction *system_tray_action_show_window;
    Settings settings;
    PreferencesDialog preferences_dialog;
    //AboutDialog about_dialog;
    UserStream user_stream;
    UpdateProfile update_profile;
    QThread *update_name_thread = new QThread;
    UpdateNameSender update_name_sender;
};

#endif // MAINWINDOW_H
