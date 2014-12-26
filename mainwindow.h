#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "preferencesdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    PreferencesDialog m_preferencesDialog;
};

#endif // MAINWINDOW_H
