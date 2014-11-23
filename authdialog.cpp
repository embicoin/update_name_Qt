#include "authdialog.h"
#include "ui_authdialog.h"
#include "update_name_oauth.h"

#include <QUrlQuery>
#include <QUrl>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QTimer>
#include <QEventLoop>
#include <QMessageAuthenticationCode>
#include <QMessageBox>
#include <QDesktopServices>

const QString AuthDialog::REQUEST_TOKEN_URL       = "https://api.twitter.com/oauth/request_token";
const QString AuthDialog::AUTHORIZE_URL           = "https://api.twitter.com/oauth/authorize";
const QString AuthDialog::ACCESS_TOKEN_URL        = "https://api.twitter.com/oauth/access_token";
const QString AuthDialog::DEFAULT_CONSUMER_KEY    = "1cIrbCiqwd3iRV5EZVHiHnzA0";
const QString AuthDialog::DEFAULT_CONSUMER_SECRET = "VYTYLtKC1p8bNUiakIEMiAvQTRSXsvRo7nBv7Srh5hvmmX7Pf9";

AuthDialog::AuthDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AuthDialog)
{
    ui->setupUi(this);

    setFixedSize(sizeHint());

    try {
        ui->urlLine->setText(this->authorizeUrl());
    } catch(std::runtime_error &e) {
        QMessageBox::critical(this,
                              tr("エラー"),
                              tr("認証ページのURLの取得に失敗しました。\n%1").arg(e.what()),
                              QMessageBox::Ok);
        QMetaObject::invokeMethod(this, "reject", Qt::QueuedConnection);
    }

    connect(ui->openUrlButton, &QPushButton::clicked, [&]() {
        if(!QDesktopServices::openUrl(QUrl(ui->urlLine->text()))) {
            QMessageBox::warning(this,
                                 tr("エラー"),
                                 tr("ブラウザを開けませんでした。\n"
                                    "手動でURLをコピーアンドペーストして認証ページを開いてください。"),
                                 QMessageBox::Ok);
        }
    });
    connect(ui->buttonBox, &QDialogButtonBox::accepted, [&]() {
        try {
            this->authorizePin(ui->pinCodeLine->text());
            this->accept();
        } catch(std::runtime_error &e) {
            QMessageBox::critical(this,
                                  tr("エラー"),
                                  tr("認証に失敗しました。\n%1").arg(e.what()),
                                  QMessageBox::Ok);
            this->reject();
            return;
        }
    });
}

AuthDialog::~AuthDialog()
{
    delete ui;
}

QString AuthDialog::authorizeUrl()
{
    const QByteArray signatureKey = DEFAULT_CONSUMER_SECRET.toUtf8() + "&";
    QVariantMap params;
    QByteArray signatureBaseString;
    QUrlQuery query;
    QUrl request_url;
    QUrlQuery tokenQuery;
    QNetworkAccessManager manager;
    QNetworkReply *reply;
    QTimer timer;
    QEventLoop loop;
    QByteArray response;
    QNetworkReply::NetworkError error;
    QString errorString;

    params["oauth_consumer_key"]     = DEFAULT_CONSUMER_KEY;
    params["oauth_nonce"]            = OAUTH_NONCE;
    params["oauth_signature_method"] = OAUTH_SIGNATURE_METHOD;
    params["oauth_timestamp"]        = OAUTH_TIMESTAMP;
    params["oauth_version"]          = OAUTH_VERSION;

    for(int i = 0; i < params.size(); i++) {
        query.addQueryItem(params.keys()[i], params.values()[i].toString());
    }

    signatureBaseString = "GET&"
            + REQUEST_TOKEN_URL.toUtf8().toPercentEncoding()
            + "&"
            + query.toString().toUtf8().toPercentEncoding();

    query.addQueryItem("oauth_signature",
                       QMessageAuthenticationCode::hash(signatureBaseString,
                                                        signatureKey,
                                                        QCryptographicHash::Sha1).toBase64().toPercentEncoding());

    request_url.setUrl(REQUEST_TOKEN_URL);
    request_url.setQuery(query);
    reply = manager.get(QNetworkRequest(request_url));

    timer.setSingleShot(true);
    connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    timer.start(15000);
    loop.exec();

    response = reply->readAll();
    error = reply->error();
    errorString = reply->errorString();
    reply->deleteLater();

    if(!timer.isActive()) {
        throw std::runtime_error("タイムアウトしました。");
    } else if(error == QNetworkReply::NoError) {
        tokenQuery.setQuery(response);
        m_requestToken = tokenQuery.queryItemValue("oauth_token");
        m_requestTokenSecret = tokenQuery.queryItemValue("oauth_token_secret");
    } else {
        if(response.isEmpty()) {
            throw std::runtime_error(errorString.toStdString());
        } else {
            throw std::runtime_error(QString::fromUtf8(response).toStdString());
        }
    }

    return AUTHORIZE_URL + "?oauth_token=" + m_requestToken;
}

void AuthDialog::authorizePin(const QString pin)
{
    if(m_requestToken.isEmpty() || m_requestTokenSecret.isEmpty()) {
        return;
    }

    const QByteArray signatureKey = DEFAULT_CONSUMER_SECRET.toUtf8() + "&";
    QVariantMap params;
    QByteArray signatureBaseString;
    QUrlQuery query;
    QUrl request_url;
    QNetworkAccessManager manager;
    QNetworkReply *reply;
    QTimer timer;
    QEventLoop loop;
    QUrlQuery tokenQuery;
    QByteArray response;
    QNetworkReply::NetworkError error;
    QString errorString;

    params["oauth_consumer_key"]     = DEFAULT_CONSUMER_KEY;
    params["oauth_nonce"]            = OAUTH_NONCE;
    params["oauth_signature_method"] = OAUTH_SIGNATURE_METHOD;
    params["oauth_timestamp"]        = OAUTH_TIMESTAMP;
    params["oauth_version"]          = OAUTH_VERSION;
    params["oauth_token"]            = m_requestToken;
    params["oauth_verifier"]         = pin;

    for(int i = 0; i < params.size(); i++) {
        query.addQueryItem(params.keys()[i], params.values()[i].toString());
    }
    signatureBaseString = "GET&"
            + ACCESS_TOKEN_URL.toUtf8().toPercentEncoding()
            + query.toString(QUrl::FullyEncoded).toUtf8();

    query.addQueryItem("oauth_signature",
                       QMessageAuthenticationCode::hash(signatureBaseString,
                                                        signatureKey,
                                                        QCryptographicHash::Sha1).toBase64().toPercentEncoding());

    request_url.setUrl(ACCESS_TOKEN_URL);
    request_url.setQuery(query);
    reply = manager.get(QNetworkRequest(request_url));

    timer.setSingleShot(true);
    connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    timer.start(15000);
    loop.exec();

    response = reply->readAll();
    error = reply->error();
    errorString = reply->errorString();
    reply->deleteLater();

    if(!timer.isActive()) {
        throw std::runtime_error("タイムアウトしました。");
    } else if(error == QNetworkReply::NoError) {
        tokenQuery.setQuery(response);
        m_settings.setConsumerKey(DEFAULT_CONSUMER_KEY);
        m_settings.setConsumerSecret(DEFAULT_CONSUMER_SECRET);
        m_settings.setAccessToken(tokenQuery.queryItemValue("oauth_token"));
        m_settings.setAccessTokenSecret(tokenQuery.queryItemValue("oauth_token_secret"));
    } else {
        if(response.isEmpty()) {
            throw std::runtime_error(errorString.toStdString());
        } else {
            throw std::runtime_error(QString::fromUtf8(response).toStdString());
        }
    }
}
