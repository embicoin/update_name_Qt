#include "mainwindow.h"
#include "ui_mainwindow.h"

#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif

#include <QMessageBox>
#include <QFileDialog>
#include <QStandardPaths>
#include <QFile>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
#ifdef Q_OS_ANDROID
    ui->saveLogAction->setVisible(false);
#endif

    //connect
    //ログのクリア
    connect(ui->clearLogAction, SIGNAL(triggered()), ui->logText, SLOT(clear()));
    //ログの保存
    connect(ui->saveLogAction, &QAction::triggered, [&]() {
        QFileDialog dialog(this, tr("ログファイルの保存"), QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
        dialog.selectFile("update_name_Qt_log.txt");
        dialog.setAcceptMode(QFileDialog::AcceptSave);

        if (dialog.exec() != QFileDialog::Accepted)
            return;

        QFile file(dialog.selectedFiles().first());
        if (file.open(QFile::WriteOnly | QFile::Text))
            if (file.write(ui->logText->toPlainText().toUtf8())) {
                QMessageBox::information(this, tr("保存しました"), tr("ログファイルを\n%1\nに保存しました。").arg(file.fileName()), QMessageBox::Ok);
                return;
            }
        QMessageBox::critical(this, tr("エラー"), tr("ファイルの保存に失敗しました。\n%1").arg(file.errorString()), QMessageBox::Ok);
    });
    //終了
    connect(ui->quitAction, SIGNAL(triggered()), QApplication::instance(), SLOT(quit()));
    //設定
    connect(ui->preferencesAction, SIGNAL(triggered()), &m_preferencesDialog, SLOT(exec()));

    //Qtについて
    connect(ui->aboutQtAction, &QAction::triggered, [&]() {
        QMessageBox::aboutQt(this, tr("Qtについて"));
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
