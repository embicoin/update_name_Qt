#include "oauth.h"
#include "../twitterglobal.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QTimer>
#include <QEventLoop>
#include <QMessageAuthenticationCode>
#include <QUrlQuery>
#include <QMap>
#include <memory>
#include <QDebug>

using namespace TwitterAPI;

const QUrl OAuth::REQUEST_TOKEN_URL("https://api.twitter.com/oauth/request_token");
const QUrl OAuth::AUTHORIZE_URL("https://api.twitter.com/oauth/authorize");
const QUrl OAuth::ACCESS_TOKEN_URL("https://api.twitter.com/oauth/access_token");

OAuth::OAuth(QObject *parent) :
    QObject(parent)
{
    initialize();
}

OAuth::OAuth(const QString &consumerKey, const QString &consumerSecret, QObject *parent) :
    QObject(parent)
{
    m_consumerKey = consumerKey;
    m_consumerSecret = consumerSecret;
    initialize();
}

OAuth::OAuth(const QString &consumerKey, const QString &consumerSecret,
             const QString &accessToken, const QString &accessTokenSecret, QObject *parent) :
    QObject(parent)
{
    m_consumerKey = consumerKey;
    m_consumerSecret = consumerSecret;
    m_accessToken = accessToken;
    m_accessTokenSecret = accessTokenSecret;
    initialize();
}

void OAuth::initialize()
{
    connect(this, SIGNAL(authorizeError(QNetworkReply::NetworkError,QString)), this, SIGNAL(authorizeFinished()));
    connect(this, SIGNAL(authorizeSuccessed(QString,QString,QString,QString)), this, SIGNAL(authorizeFinished()));
    connect(this, SIGNAL(requestTokenError(QNetworkReply::NetworkError,QString)), this, SIGNAL(requestTokenFinished()));
    connect(this, SIGNAL(requestTokenRecieved(QUrl)), this, SIGNAL(requestTokenFinished()));
}

QString OAuth::consumerKey() const
{
    return m_consumerKey;
}

QString OAuth::consumerSecret() const
{
    return m_consumerSecret;
}

QString OAuth::accessToken() const
{
    return m_accessToken;
}

QString OAuth::accessTokenSecret() const
{
    return m_accessTokenSecret;
}

QString OAuth::userId() const
{
    return m_userId;
}

QString OAuth::screenName() const
{
    return m_screenName;
}

void OAuth::setConsumerKey(const QString &consumerKey)
{
    m_consumerKey = consumerKey;
}

void OAuth::setConsumerSecret(const QString &consumerSecret)
{
    m_consumerSecret = consumerSecret;
}

void OAuth::setAccessToken(const QString &accessToken)
{
    m_accessToken = accessToken;
}

void OAuth::setAccessTokenSecret(const QString &accessTokenSecret)
{
    m_accessTokenSecret = accessTokenSecret;
}

QUrl OAuth::authorizeUrl()
{
    const QByteArray signatureKey = m_consumerSecret.toUtf8() + "&";
    QVariantMap oauthParams;
    QByteArray signatureBaseString;
    QUrl requestUrl;
    QUrlQuery query;
    QNetworkAccessManager manager;
    std::unique_ptr<QNetworkReply> reply;
    QTimer timer;
    QEventLoop loop;
    QByteArray response;

    m_requestToken.clear();
    m_requestTokenSecret.clear();

    oauthParams["oauth_consumer_key"]     = m_consumerKey;
    oauthParams["oauth_nonce"]            = OAUTH_NONCE;
    oauthParams["oauth_signature_method"] = OAUTH_SIGNATURE_METHOD;
    oauthParams["oauth_timestamp"]        = OAUTH_TIMESTAMP;
    oauthParams["oauth_version"]          = OAUTH_VERSION;

    for (QVariantMap::const_iterator i = oauthParams.constBegin(); i != oauthParams.constEnd(); i++)
        query.addQueryItem(i.key(), i.value().toByteArray().toPercentEncoding());

    signatureBaseString = "GET&" + REQUEST_TOKEN_URL.toString().toUtf8().toPercentEncoding()
            + "&" + query.toString(QUrl::FullyEncoded).toUtf8().toPercentEncoding();

    query.addQueryItem("oauth_signature",
                       QMessageAuthenticationCode::hash(signatureBaseString,
                                                        signatureKey,
                                                        QCryptographicHash::Sha1).toBase64().toPercentEncoding());

    requestUrl = REQUEST_TOKEN_URL;
    requestUrl.setQuery(query);
    reply.reset(manager.get(QNetworkRequest(requestUrl)));

    timer.setSingleShot(true);
    connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
    connect(reply.get(), SIGNAL(finished()), &loop, SLOT(quit()));
    timer.start(TIMEOUT_COUNT_MSEC);
    loop.exec();

    response = reply->readAll();

    if(!timer.isActive()) {
        emit requestTokenError(QNetworkReply::TimeoutError, NULL);
        return QUrl(NULL);
    } else if(reply->error() != QNetworkReply::NoError) {
        emit requestTokenError(reply->error(),
                               response.isEmpty() ? reply->errorString() : response);
        return QUrl(NULL);
    }

    QUrl authorizeUrl = AUTHORIZE_URL;
    QUrlQuery tokenQuery;

    tokenQuery.setQuery(response);

    m_requestToken       = tokenQuery.queryItemValue("oauth_token");
    m_requestTokenSecret = tokenQuery.queryItemValue("oauth_token_secret");

    if (m_requestToken.isEmpty() || m_requestTokenSecret.isEmpty()) {
        emit requestTokenError(QNetworkReply::UnknownNetworkError, QString::fromUtf8(response));
        return QUrl(NULL);
    }

    authorizeUrl.setQuery(tokenQuery);

    emit requestTokenRecieved(authorizeUrl);
    return authorizeUrl;
}

void OAuth::authorizePin(const QString &pinCode)
{
    const QByteArray signatureKey = m_consumerSecret.toUtf8() + "&" + m_requestTokenSecret.toUtf8();
    QVariantMap oauthParams;
    QByteArray signatureBaseString;
    QUrl requestUrl;
    QUrlQuery query;
    QNetworkAccessManager manager;
    std::unique_ptr<QNetworkReply> reply;
    QTimer timer;
    QEventLoop loop;
    QByteArray response;

    oauthParams["oauth_consumer_key"]     = m_consumerKey;
    oauthParams["oauth_nonce"]            = OAUTH_NONCE;
    oauthParams["oauth_signature_method"] = OAUTH_SIGNATURE_METHOD;
    oauthParams["oauth_timestamp"]        = OAUTH_TIMESTAMP;
    oauthParams["oauth_version"]          = OAUTH_VERSION;
    oauthParams["oauth_token"]            = m_requestToken;
    oauthParams["oauth_verifier"]         = pinCode;

    for(QVariantMap::const_iterator i = oauthParams.constBegin(); i != oauthParams.constEnd(); i++)
        query.addQueryItem(i.key(), i.value().toByteArray().toPercentEncoding());

    signatureBaseString = "GET&" + ACCESS_TOKEN_URL.toString().toUtf8().toPercentEncoding()
            + "&" + query.toString(QUrl::FullyEncoded).toUtf8();

    query.addQueryItem("oauth_signature",
                       QMessageAuthenticationCode::hash(signatureBaseString,
                                                        signatureKey,
                                                        QCryptographicHash::Sha1).toBase64().toPercentEncoding());

    requestUrl = ACCESS_TOKEN_URL;
    requestUrl.setQuery(query);
    reply.reset(manager.get(QNetworkRequest(requestUrl)));

    timer.setSingleShot(true);
    connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
    connect(reply.get(), SIGNAL(finished()), &loop, SLOT(quit()));
    timer.start(TIMEOUT_COUNT_MSEC);

    loop.exec();

    response = reply->readAll();

    if (!timer.isActive()) {
        emit authorizeError(QNetworkReply::TimeoutError, NULL);
    } else if (reply->error() != QNetworkReply::NoError) {
        emit authorizeError(reply->error(),
                            response.isEmpty() ? reply->errorString() : response);
    } else {
        QUrlQuery tokenQuery;
        tokenQuery.setQuery(response);
        m_accessToken = tokenQuery.queryItemValue("oauth_token");
        m_accessTokenSecret = tokenQuery.queryItemValue("oauth_token_secret");
        m_userId = tokenQuery.queryItemValue("user_id");
        m_screenName = tokenQuery.queryItemValue("screen_name");
        if(m_accessToken.isEmpty() || m_accessToken.isEmpty())
            emit authorizeError(QNetworkReply::UnknownNetworkError, response);
        else
            emit authorizeSuccessed(m_accessToken, m_accessTokenSecret, m_userId, m_screenName);
    }
}
