#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDialog>
//#include <QCloseEvent>
//#include <QKeyEvent>

namespace Ui {
class PreferencesDialog;
}

class PreferencesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PreferencesDialog(QWidget *parent = 0);
    ~PreferencesDialog();

private:
    //void closeEvent(QCloseEvent *);
    //void keyReleaseEvent(QKeyEvent *event);
    void loadSettings();
    void saveSettings();

    static const QSize ICON_SIZE;
    Ui::PreferencesDialog *ui;
};

#endif // PREFERENCESDIALOG_H
