#include "authdialog.h"
#include "ui_authdialog.h"
#include "../updatenameqtglobal.h"

#include <QDesktopServices>
#include <QMessageBox>

AuthDialog::AuthDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AuthDialog)
{
    ui->setupUi(this);
    setWindowState(Qt::WindowMaximized);

    //SIGNALの接続
    //認証ページを開く
    connect(ui->openAuthPageButton, &QPushButton::clicked, [&]() {
        QDesktopServices::openUrl(QUrl(ui->urlLine->text()));
    });
    //PAIキーの設定
    connect(ui->apiKeyConfigButton, &QPushButton::clicked, [&]() {
        ui->stackedWidget->setCurrentIndex(1);
    });
    //PINコード入力欄
    connect(ui->pinCodeLine, &QLineEdit::textChanged, [&](const QString &text) {
        ui->enterPinCodeButton->setEnabled(text.length() == 7);
    });
    //入力
    connect(ui->enterPinCodeButton, &QPushButton::clicked, [&]() {
        m_oauth.authorizePin(ui->pinCodeLine->text());
    });
    //APIキー設定の画面のボタン
    connect(ui->apiKeyConfigButtonBox, &QDialogButtonBox::clicked, [&](QAbstractButton *button) {
        switch(ui->apiKeyConfigButtonBox->standardButton(button)) {
        case QDialogButtonBox::Help: {
            QMessageBox helpMessage(QMessageBox::NoIcon, tr("APIキーとは"),
                                    tr("APIキーはConsumerKeyとConsumerSecretがあり、認証に使います。\n"
                                       "通常は変更する必要はありませんが、自分でAPIキーをセットすることができます。\n"
                                       "APIキーは https://dev.twitter.com/ で取得することができます。"), QMessageBox::Ok, this);
            helpMessage.setIconPixmap(QApplication::style()->standardPixmap(QStyle::SP_DialogHelpButton));
            helpMessage.exec();
        }
            break;
        case QDialogButtonBox::Ok:
            m_oauth.setConsumerKey(ui->consumerKeyLine->text());
            m_oauth.setConsumerSecret(ui->consumerSecretLine->text());
            ui->urlLine->setText(m_oauth.authorizeUrl().toString());
            ui->stackedWidget->setCurrentIndex(0);
            break;
        case QDialogButtonBox::Cancel:
            ui->consumerKeyLine->setText(m_oauth.consumerKey());
            ui->consumerSecretLine->setText(m_oauth.consumerSecret());
            ui->stackedWidget->setCurrentIndex(0);
            break;
        case QDialogButtonBox::RestoreDefaults:
            ui->consumerKeyLine->setText(UpdateNameQt::defaultSettings["ConsumerKey"].toString());
            ui->consumerSecretLine->setText(UpdateNameQt::defaultSettings["ConsumerSecret"].toString());
            break;
        default:
            break;
        }
    });
    //OAuth
    connect(&m_oauth, &OAuth::requestTokenError, [&](QNetworkReply::NetworkError, const QString &errorResponse) {
        QMessageBox::critical(this, tr("エラー"), tr("認証ページを開くのに失敗しました。\n"
                                                  "もう一度やり直してください。\n"
                                                  "%1").arg(errorResponse), QMessageBox::Ok);
    });
    connect(&m_oauth, &OAuth::authorizeError, [&](QNetworkReply::NetworkError, const QString &errorResponse) {
        QMessageBox::critical(this, tr("エラー"), tr("認証に失敗しました。\n"
                                                  "もう一度やり直してください。\n"
                                                  "%1").arg(errorResponse), QMessageBox::Ok);
    });
    connect(&m_oauth, &OAuth::authorizeSuccessed, [&](const QString &accessToken, const QString &accessTokenSecret, const QString &userId, const QString &screenName) {
        UpdateNameQt::settings->setValue("ConsumerKey", m_oauth.consumerKey());
        UpdateNameQt::settings->setValue("ConsumerSecret", m_oauth.consumerSecret());
        UpdateNameQt::settings->setValue("AccessToken", accessToken);
        UpdateNameQt::settings->setValue("AccessTokenSecret", accessTokenSecret);
        UpdateNameQt::settings->setValue("UserId", userId);
        QMessageBox::information(this, tr("認証が完了しました"), tr("認証が完了しました！\n"
                                                           "あなたのscreen_name：%1").arg(screenName), QMessageBox::Ok);
        done(AuthSuccessed);
    });

    //API Keyのセット
    ui->consumerKeyLine->setText(UpdateNameQt::settings->value("ConsumerKey").isNull()
                                 ? UpdateNameQt::defaultSettings["ConsumerKey"].toString()
                                 : UpdateNameQt::settings->value("ConsumerKey").toString());
    ui->consumerSecretLine->setText(UpdateNameQt::settings->value("ConsumerSecret").isNull()
                                    ? UpdateNameQt::defaultSettings["ConsumerSecret"].toString()
                                    : UpdateNameQt::settings->value("ConsumerSecret").toString());
    m_oauth.setConsumerKey(ui->consumerKeyLine->text());
    m_oauth.setConsumerSecret(ui->consumerSecretLine->text());

    //URLの表示
    ui->urlLine->setText(m_oauth.authorizeUrl().toString());
}

AuthDialog::~AuthDialog()
{
    delete ui;
}
