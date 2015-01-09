#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>

#include "preferencesdialog.h"
#include "updatename.h"

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

private:
    QString updateTypeToString(UpdateProfile::UpdateType type);

    Ui::MainWindow *ui;
    QMenuBar *m_menuBar;
    PreferencesDialog m_preferencesDialog;
    UpdateName m_updateName;
};

#endif // MAINWINDOW_H
