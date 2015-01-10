#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QSystemTrayIcon>
#include <QCloseEvent>

#include "preferencesdialog.h"
#include "updatename.h"
#include "updatenamesender.h"
#include "aboutdialog.h"

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
    void saveLog();

protected:
    void closeEvent(QCloseEvent *);

private:
    QString updateTypeToString(UpdateProfile::UpdateType type);

    Ui::MainWindow *ui;
    QMenuBar *m_menuBar;
    QSystemTrayIcon *m_systemTrayIcon;
    PreferencesDialog m_preferencesDialog;
    UpdateName m_updateName;
    UpdateNameSender m_updateNameSender;
    AboutDialog m_aboutDialog;
};

#endif // MAINWINDOW_H
