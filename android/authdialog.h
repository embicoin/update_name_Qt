#ifndef AUTHDIALOG_H
#define AUTHDIALOG_H

#include <QDialog>

#include "../twitterAPI/oauth/oauth.h"

namespace Ui {
class AuthDialog;
}

class AuthDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AuthDialog(QWidget *parent = 0);
    ~AuthDialog();

    enum Result {
        AuthSuccessed,
        Cancelled,
        Retry,
    };

private:
    Ui::AuthDialog *ui;
    OAuth m_oauth;
};

#endif // AUTHDIALOG_H
