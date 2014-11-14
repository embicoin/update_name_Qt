#ifndef AUTHDIALOG_H
#define AUTHDIALOG_H

#include "settings.h"

#include <QDialog>

namespace Ui {
class AuthDialog;
}

class AuthDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AuthDialog(QWidget *parent = 0);
    ~AuthDialog();

private:
    QString authorizeUrl();
    void authorizePin(const QString pin);

    static const QString REQUEST_TOKEN_URL;
    static const QString AUTHORIZE_URL;
    static const QString ACCESS_TOKEN_URL;
    static const QString DEFAULT_CONSUMER_KEY;
    static const QString DEFAULT_CONSUMER_SECRET;

    Ui::AuthDialog *ui;
    Settings settings;

    QString request_token;
    QString request_token_secret;
};

#endif // AUTHDIALOG_H
