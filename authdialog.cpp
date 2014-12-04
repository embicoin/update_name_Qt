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
#include <QDebug>
#include <memory>

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

    bool setFinished = false;

    while(!setFinished) {
        try {
            qDebug() << "[Info] AuthDialog: Setting authorize page url.";
            ui->urlLine->setText(authorizeUrl());
            setFinished = true;
            qDebug() << "[Info] AuthDialog: Set url finished.";
        } catch(std::runtime_error &e) {
            qCritical() << "[Error] AuthDialog: Setting authorize page url failed.\n"
                           "        Error message:" << e.what();
            switch(QMessageBox::critical(this,
                                  tr("エラー"),
                                  tr("認証ページのURLの取得に失敗しました。\n%1").arg(e.what()),
                                  QMessageBox::Ok, QMessageBox::Retry)) {
            case QMessageBox::Retry:
                setFinished = false;
                break;
            case QMessageBox::Ok:
            default:
                setFinished = true;
            }
        }
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
            authorizePin(ui->pinCodeLine->text());
            qDebug() << "[Info] AuthDialog: Authorize finished.";
        } catch(std::runtime_error &e) {
            qCritical() << "[Error] AuthDialog: Authorize failed.\n"
                           "        Error message:" << e.what();
            switch(QMessageBox::critical(this,
                                         tr("エラー"),
                                         tr("認証に失敗しました。\n%1").arg(e.what()),
                                         QMessageBox::Ok, QMessageBox::Retry)) {
            case QMessageBox::Retry:
                QMetaObject::invokeMethod(this, "done", Q_ARG(int, 255));
                break;
            case QMessageBox::Ok:
            default:
                QMetaObject::invokeMethod(this, "reject");
                break;
            }
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
    std::unique_ptr<QNetworkReply> reply;
    QTimer timer;
    QEventLoop loop;
    QByteArray response;

    qDebug() << "[Info] AuthDialog: Clear request token.";

    m_requestToken.clear();
    m_requestTokenSecret.clear();

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

    qDebug() << "[Info] AuthDialog: Request to twitter api.";

    request_url.setUrl(REQUEST_TOKEN_URL);
    request_url.setQuery(query);
    reply.reset(manager.get(QNetworkRequest(request_url)));

    timer.setSingleShot(true);
    connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
    connect(reply.get(), SIGNAL(finished()), &loop, SLOT(quit()));
    timer.start(15000);
    loop.exec();

    response = reply->readAll();

    if(!timer.isActive()) {
        throw std::runtime_error("タイムアウトしました。");
    } else if(reply->error() == QNetworkReply::NoError) {
        tokenQuery.setQuery(response);
        m_requestToken = tokenQuery.queryItemValue("oauth_token");
        m_requestTokenSecret = tokenQuery.queryItemValue("oauth_token_secret");
    } else {
        if(response.isEmpty()) {
            throw std::runtime_error(QString::fromUtf8(reply->readAll()).toStdString());
        } else {
            throw std::runtime_error(QString::fromUtf8(response).toStdString());
        }
    }

    return AUTHORIZE_URL + "?oauth_token=" + m_requestToken;
}

void AuthDialog::authorizePin(const QString pin)
{
    const QByteArray signatureKey = DEFAULT_CONSUMER_SECRET.toUtf8() + "&";
    QVariantMap params;
    QByteArray signatureBaseString;
    QUrlQuery query;
    QUrl request_url;
    QNetworkAccessManager manager;
    std::unique_ptr<QNetworkReply> reply;
    QTimer timer;
    QEventLoop loop;
    QUrlQuery tokenQuery;
    QByteArray response;

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

    qDebug() << "[Info] AuthDialog: Request to twitter api.";

    request_url.setUrl(ACCESS_TOKEN_URL);
    request_url.setQuery(query);
    reply.reset(manager.get(QNetworkRequest(request_url)));

    timer.setSingleShot(true);
    connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
    connect(reply.get(), SIGNAL(finished()), &loop, SLOT(quit()));
    timer.start(15000);
    loop.exec();

    response = reply->readAll();

    if(!timer.isActive()) {
        throw std::runtime_error("タイムアウトしました。");
    } else if(reply->error() == QNetworkReply::NoError) {
        tokenQuery.setQuery(response);
        m_settings.setConsumerKey(DEFAULT_CONSUMER_KEY);
        m_settings.setConsumerSecret(DEFAULT_CONSUMER_SECRET);
        m_settings.setAccessToken(tokenQuery.queryItemValue("oauth_token"));
        m_settings.setAccessTokenSecret(tokenQuery.queryItemValue("oauth_token_secret"));
        m_settings.setUserId(tokenQuery.queryItemValue("user_id"));
        m_settings.setScreenName(tokenQuery.queryItemValue("screen_name"));
    } else {
        if(response.isEmpty()) {
            throw std::runtime_error(reply->errorString().toStdString());
        } else {
            throw std::runtime_error(QString::fromUtf8(response).toStdString());
        }
    }
}
