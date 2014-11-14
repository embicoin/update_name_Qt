#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include "settings.h"

#include <QDialog>

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
    Ui::PreferencesDialog *ui;
    Settings settings;
};

#endif // PREFERENCESDIALOG_H
