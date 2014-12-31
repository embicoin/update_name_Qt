#include "restapi.h"
#include "../twitterglobal.h"
#include "../object/error.h"

#include <QNetworkRequest>
#include <QTimer>
#include <QEventLoop>
#include <QUrlQuery>
#include <QMessageAuthenticationCode>
#include <memory>
#include <QDebug>

using namespace TwitterAPI::Rest;

RestApi::RestApi(QObject *parent) :
    QObject(parent)
{
}

RestApi::RestApi(const TwitterAPI::OAuth &oauth, QObject *parent) :
    QObject(parent)
{
    m_consumerKey       = oauth.consumerKey().toUtf8();
    m_consumerSecret    = oauth.consumerSecret().toUtf8();
    m_accessToken       = oauth.accessToken().toUtf8();
    m_accessTokenSecret = oauth.accessTokenSecret().toUtf8();
}

void RestApi::setOAuth(const TwitterAPI::OAuth &oauth)
{
    m_consumerKey       = oauth.consumerKey().toUtf8();
    m_consumerSecret    = oauth.consumerSecret().toUtf8();
    m_accessToken       = oauth.accessToken().toUtf8();
    m_accessTokenSecret = oauth.accessTokenSecret().toUtf8();
}

QString RestApi::errorString()
{
    return m_errorMessage;
}

int RestApi::responseCode()
{
    return m_responseCode;
}

QByteArray RestApi::requestApi(const QNetworkAccessManager::Operation &httpMethod,
                                   QUrl resourceUrl, const QVariantMap &parameters)
{
    const QByteArray signatureKey = m_consumerSecret + "&" + m_accessTokenSecret;
    QByteArray httpMethodString;
    QVariantMap oauthParams;
    QVariantMap signatureParams;
    QVariantMap::const_iterator i;
    QUrlQuery signatureQuery;
    QByteArray signatureBaseString;
    QByteArray oauthHeader;
    QUrlQuery parametersQuery;
    QNetworkRequest request;
    QNetworkAccessManager manager;
    std::unique_ptr<QNetworkReply> reply;
    QTimer timer;
    QEventLoop loop;

    m_errorMessage.clear();
    m_responseCode = 0;

    switch (httpMethod) {
    case QNetworkAccessManager::GetOperation:
        httpMethodString = "GET";
        break;
    case QNetworkAccessManager::PostOperation:
        httpMethodString = "POST";
        break;
    default:
        return 0;
    }

    oauthParams["oauth_consumer_key"]     = m_consumerKey;
    oauthParams["oauth_token"]            = m_accessToken;
    oauthParams["oauth_signature_method"] = OAUTH_SIGNATURE_METHOD;
    oauthParams["oauth_timestamp"]        = OAUTH_TIMESTAMP;
    oauthParams["oauth_nonce"]            = OAUTH_NONCE;
    oauthParams["oauth_version"]          = OAUTH_VERSION;

    signatureParams = oauthParams;

    if (!parameters.isEmpty())
        for (i = parameters.constBegin(); i != parameters.constEnd(); i++) {
            signatureParams.insert(i.key(), i.value().toByteArray().toPercentEncoding());
            parametersQuery.addQueryItem(i.key(), i.value().toByteArray().toPercentEncoding());
        }

    for (QVariantMap::const_iterator i = signatureParams.constBegin(); i != signatureParams.constEnd(); i++)
        signatureQuery.addQueryItem(i.key(), i.value().toString());

    signatureBaseString = httpMethodString + "&" + resourceUrl.toString().toUtf8().toPercentEncoding()
            + "&" + signatureQuery.toString(QUrl::FullyEncoded).toUtf8().toPercentEncoding();

    oauthParams["oauth_signature"]
            = QMessageAuthenticationCode::hash(signatureBaseString,
                                               signatureKey,
                                               QCryptographicHash::Sha1).toBase64().toPercentEncoding();
    oauthHeader = "OAuth ";
    for (i = oauthParams.begin(); i != oauthParams.end(); i++)
        oauthHeader.append(i.key() + "=\"" + i.value().toByteArray() + "\", ");
    oauthHeader.chop(2);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setRawHeader("Authorization", oauthHeader);

    switch (httpMethod) {
    case QNetworkAccessManager::GetOperation:
        if (!parametersQuery.isEmpty())
            resourceUrl.setQuery(parametersQuery);
        request.setUrl(resourceUrl);
        reply.reset(manager.get(request));
        break;
    case QNetworkAccessManager::PostOperation:
        request.setUrl(resourceUrl);
        reply.reset(manager.post(request, parametersQuery.toString(QUrl::FullyEncoded).toUtf8()));
        break;
    default:
        break;
    }

    connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
    connect(reply.get(), SIGNAL(finished()), &loop, SLOT(quit()));
    timer.setSingleShot(true);
    timer.start(TIMEOUT_COUNT_MSEC);
    loop.exec();

    if (!reply->isFinished()) {
        m_errorMessage = tr("タイムアウトしました。");
        throw std::runtime_error(tr("タイムアウトしました。").toStdString());
    } else {
        QByteArray response = reply->readAll();
        m_responseCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        if (reply->error() != QNetworkReply::NoError) {
            if (response.isEmpty()) {
                m_errorMessage = reply->errorString();
                throw std::runtime_error(reply->errorString().toStdString());
            } else {
                m_errorMessage = TwitterAPI::Object::Error(response).message();
                throw TwitterAPI::Object::Error(response);
            }
        } else {
            m_errorMessage.clear();
            return response;
        }
    }
}
