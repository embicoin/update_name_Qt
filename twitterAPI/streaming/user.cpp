#include "user.h"
#include "../twitterglobal.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrlQuery>
#include <QEventLoop>
#include <QTimer>
#include <QMessageAuthenticationCode>
#include <memory>

using namespace TwitterAPI::Streaming;

UserParameters::UserParameters()
{
    qRegisterMetaType<TwitterAPI::Streaming::UserParameters>("TwitterAPI::Streaming::UserParameters");
}

const QUrl User::USERSTREAM_URL("https://userstream.twitter.com/1.1/user.json");

User::User(const TwitterAPI::OAuth &oauth, QObject *parent)
    : QThread(parent)
{
    m_consumerKey = oauth.consumerKey().toUtf8();
    m_consumerSecret = oauth.consumerSecret().toUtf8();
    m_accessToken = oauth.accessToken().toUtf8();
    m_accessTokenSecret = oauth.accessTokenSecret().toUtf8();
}

User::~User()
{

}

void User::run()
{
    m_stopped = false;

    while (!m_stopped) {
        const QByteArray signatureKey = m_consumerSecret + "&" + m_accessTokenSecret;
        QVariantMap oauthParams;
        QUrlQuery oauthQuery;
        QByteArray oauthHeader;
        QByteArray signatureBaseString;
        QNetworkRequest request;
        QNetworkAccessManager manager;
        std::unique_ptr<QNetworkReply> reply;
        QByteArray response;
        QByteArray buffer;
        QEventLoop loop;
        QTimer timer;
        bool connected = false;
        uint waitCount = 1;
        QEventLoop waitLoop;
        QTimer waitTimer;

        //接続開始
        emit connectionStarted();

        //パラメータの成形
        oauthParams["oauth_consumer_key"]     = m_consumerKey;
        oauthParams["oauth_token"]            = m_accessToken;
        oauthParams["oauth_signature_method"] = OAUTH_SIGNATURE_METHOD;
        oauthParams["oauth_timestamp"]        = OAUTH_TIMESTAMP;
        oauthParams["oauth_nonce"]            = OAUTH_NONCE;
        oauthParams["oauth_version"]          = OAUTH_VERSION;

        //パラメータのクエリにする
        for (auto i = oauthParams.constBegin(); i != oauthParams.constEnd(); i++)
            oauthQuery.addQueryItem(i.key(), i.value().toString());

        //Signature base stringの成形
        signatureBaseString = "GET&"
                + USERSTREAM_URL.toString().toUtf8().toPercentEncoding()
                + "&" + oauthQuery.toString().toUtf8().toPercentEncoding();

        //署名する
        oauthParams["oauth_signature"] = QMessageAuthenticationCode::hash(signatureBaseString,
                                                                          signatureKey,
                                                                          QCryptographicHash::Sha1).toBase64().toPercentEncoding();
        //ヘッダの成形
        oauthHeader = "OAuth ";
        for (auto i = oauthParams.constBegin(); i != oauthParams.constEnd(); i++)
            oauthHeader.append(i.key() + "=\"" + i.value().toByteArray() + "\", ");
        oauthHeader.chop(2);

        //ヘッダのセット
        request.setUrl(USERSTREAM_URL);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
        request.setRawHeader("Authorization", oauthHeader);

        //TwitterAPIにリクエストする
        reply.reset(manager.get(request));

        connect(reply.get(), SIGNAL(readyRead()), &loop, SLOT(quit()));
        connect(reply.get(), SIGNAL(finished()), &loop, SLOT(quit()));
        connect(reply.get(), SIGNAL(error(QNetworkReply::NetworkError)), &loop, SLOT(quit()));
        connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
        connect(this, SIGNAL(stopping()), &loop, SLOT(quit()));
        connect(this, SIGNAL(stopping()), reply.get(), SLOT(abort()));

        timer.setSingleShot(true);
        buffer.clear();

        for(;;) {
            //タイムアウト判定用のタイマーをセット
            timer.start(TIMEOUT_COUNT_MSEC);
            //イベントループの開始
            loop.exec();
            //stopメソッドが呼ばれた場合
            if (m_stopped) {
                //切断
                emit disConnected();
                return;
            //エラーが発生した場合
            } else if (reply->error() != QNetworkReply::NoError && reply->error() != QNetworkReply::OperationCanceledError) {
                //エラー
                emit error(reply->errorString());
                //切断
                emit disConnected();
                break;
            //タイムアウトした場合
            } else if (!timer.isActive() || reply->isFinished()) {
                emit disConnected();
                break;
            } else {
                if (!connected) {
                    //ユーザーストリーム開始
                    emit started();
                    //接続済みにする
                    connected = true;
                    //待機時間のリセット
                    waitCount = 1;
                }
                response = reply->readAll();

                int index = response.indexOf("\r");

                if (index != 0) {
                    if (index == -1) {
                        buffer.append(response);
                    } else {
                        response.prepend(buffer);
                        buffer.clear();
                        auto *parseJson = new ParseJson(response);
                        connect(parseJson, SIGNAL(tweet(TwitterAPI::Object::Tweets)), this, SIGNAL(recievedTweet(TwitterAPI::Object::Tweets)));
                        connect(parseJson, SIGNAL(statusDeletion(TwitterAPI::Streaming::StatusDeletionNotices)),
                                this, SIGNAL(recievedStatusDeletion(TwitterAPI::Streaming::StatusDeletionNotices)));
                        connect(parseJson, SIGNAL(finished()), parseJson, SLOT(deleteLater()));
                        parseJson->start();
                    }
                }
            }
        }
        //切断
        connected = false;
        //待機
        emit wait(waitCount);
        connect(&waitTimer, SIGNAL(timeout()), &waitLoop, SLOT(quit()));
        connect(this, SIGNAL(stopping()), &waitLoop, SLOT(quit()));
        waitTimer.setSingleShot(true);
        waitTimer.start(waitCount * 1000);
        waitLoop.exec();
        //待機時間を倍にする
        if (waitCount <= 300)
            waitCount *= 2;
    }
}

void User::stop()
{
    emit stopping();
    m_stopped = true;
}
