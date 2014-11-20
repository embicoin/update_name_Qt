#ifndef UPDATENAMESENDER_H
#define UPDATENAMESENDER_H

#include "twitter/restclient.h"

#include <QDialog>

namespace Ui {
class UpdateNameSender;
}

class UpdateNameSender : public QDialog
{
    Q_OBJECT

public:
    explicit UpdateNameSender(QWidget *parent = 0);
    ~UpdateNameSender();

private:
    Ui::UpdateNameSender *ui;
    RestClient twitter;
};

#endif // UPDATENAMESENDER_H
