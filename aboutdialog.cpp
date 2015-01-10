#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    ui->label_2->setText(ui->label_2->text().replace("${version}", QApplication::applicationVersion()));
    ui->icon->setPixmap(QPixmap(":/icon/icon/icon.png"));
    setFixedSize(sizeHint());
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
