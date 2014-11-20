#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include "settings.h"

#include <QDialog>
#include <QShowEvent>
#include <QCloseEvent>

namespace Ui {
class PreferencesDialog;
}

class PreferencesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PreferencesDialog(QWidget *parent = 0);
    ~PreferencesDialog();

private slots:
    void loadSettings();
    void saveSettings();

private:
    void showEvent(QShowEvent *);
    void closeEvent(QCloseEvent *);
    Ui::PreferencesDialog *ui;
    Settings settings;
};

#endif // PREFERENCESDIALOG_H
