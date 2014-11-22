#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//#include "aboutdialog.h"
#include "preferencesdialog.h"
#include "twitter/userstream.h"
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

private slots:
    void writeLog(const QString &log);
    void writeWelcomeLog();
    void writeUserStreamLog(UserStream::State state);
    void writeUpdateProfileLog(Update::State state, UpdateProfile::ProfileType type);
    void saveLog();
    void startUpdateName();
    void stopUpdateName();
    void quitUpdateNameQt();

private:
    void closeEvent(QCloseEvent *);

    Ui::MainWindow    *ui;
    QSystemTrayIcon   *m_systemTray;
    QMenu             *m_systemTrayMenu;
    QAction           *m_systemTrayActionQuit;
    QAction           *m_systemTrayActionShowWindow;
    Settings          m_settings;
    PreferencesDialog m_preferencesDialog;
    UserStream        m_userStream;
    UpdateProfile     m_updateProfile;
    QThread           *m_updateNameThread = new QThread;
    UpdateNameSender  m_updateNameSender;
};

#endif // MAINWINDOW_H
