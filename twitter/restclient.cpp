#include "restclient.h"
#include "../update_name_oauth.h"
#include "usersobject.h"

#include <QUrlQuery>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QTimer>
#include <QEventLoop>
#include <QMessageAuthenticationCode>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>

const QString RestClient::ACCOUNT_VERIFY_CREDENTIALS_URL              = "https://api.twitter.com/1.1/account/verify_credentials.json";
const QString RestClient::ACCOUNT_UPDATE_PROFILE_URL                  = "https://api.twitter.com/1.1/account/update_profile.json";
const QString RestClient::STATUSES_UPDATE_URL                         = "https://api.twitter.com/1.1/statuses/update.json";
const QString RestClient::MEDIA_UPLOAD_URL                            = "https://upload.twitter.com/1.1/media/upload.json";
const QString RestClient::ACCOUT_UPDATE_PROFILE_IMAGE_URL             = "https://api.twitter.com/1.1/account/update_profile_image.json";
const QString RestClient::USERS_LOOKUP_URL                            = "https://api.twitter.com/1.1/users/lookup.json";
const QString RestClient::ACCOUNT_UPDATE_PROFILE_BACKGROUND_IMAGE_URL = "https://api.twitter.com/1.1/account/update_profile_background_image.json";
const QString RestClient::ACCOUNT_UPDATE_PROFILE_BANNER_URL           = "https://api.twitter.com/1.1/account/update_profile_banner.json";

RestClient::RestClient(QObject *parent) :
    QObject(parent)
{
}

QByteArray RestClient::requestTwitterApi(const QNetworkAccessManager::Operation method,
                                      const QString &url,
                                      const QVariantMap &dataParams)
{
    const QByteArray signatureKey = m_settings.consumerSecret().toUtf8()
            + "&"
            + m_settings.accessTokenSecret().toUtf8();
    QByteArray httpMethodString;
    QVariantMap oauthParams;
    QVariantMap signatureParams;
    QUrlQuery signatureQuery;
    QByteArray signatureBaseString;
    QByteArray oauthHeader;
    QUrlQuery dataQuery;
    QUrl requestUrl(url);
    QNetworkRequest request;
    QNetworkAccessManager manager;
    QNetworkReply *reply = 0;
    QTimer timer;
    QEventLoop loop;
    QNetworkReply::NetworkError error;
    QString errorString;
    QByteArray response;

    switch (method) {
    case QNetworkAccessManager::GetOperation:
        httpMethodString = "GET";
        break;
    case QNetworkAccessManager::PostOperation:
        httpMethodString = "POST";
        break;
    default:
        throw std::runtime_error("Unknown Http method.");
        break;
    }

    oauthParams["oauth_consumer_key"]     = m_settings.consumerKey();
    oauthParams["oauth_token"]            = m_settings.accessToken();
    oauthParams["oauth_signature_method"] = OAUTH_SIGNATURE_METHOD;
    oauthParams["oauth_timestamp"]        = OAUTH_TIMESTAMP;
    oauthParams["oauth_nonce"]            = OAUTH_NONCE;
    oauthParams["oauth_version"]          = OAUTH_VERSION;

    for(int i = 0; i < oauthParams.size(); i++) {
        signatureParams.insert(oauthParams.keys()[i], oauthParams.values()[i]);
    }

    if(!dataParams.isEmpty()) {
        for(int i = 0; i < dataParams.size(); i++) {
            signatureParams.insert(dataParams.keys()[i], dataParams.values()[i].toByteArray().toPercentEncoding());
            dataQuery.addQueryItem(dataParams.keys()[i], dataParams.values()[i].toByteArray().toPercentEncoding());
        }
    }

    for(int i = 0; i < signatureParams.size(); i++) {
        signatureQuery.addQueryItem(signatureParams.keys()[i], signatureParams.values()[i].toString());
    }

    signatureBaseString = httpMethodString
            + "&"
            + url.toUtf8().toPercentEncoding()
            + "&"
            + signatureQuery.toString(QUrl::FullyEncoded).toUtf8().toPercentEncoding();

    oauthParams["oauth_signature"] = QMessageAuthenticationCode::hash(signatureBaseString,
                                                                      signatureKey,
                                                                      QCryptographicHash::Sha1).toBase64().toPercentEncoding();

    oauthHeader.append("OAuth ");
    for(int i = 0; i < oauthParams.size(); i++) {
        oauthHeader.append(oauthParams.keys()[i])
                   .append("=\"")
                   .append(oauthParams.values()[i].toByteArray())
                   .append("\", ");
    }
    oauthHeader.chop(2);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setRawHeader("Authorization", oauthHeader);

    switch (method) {
    case QNetworkAccessManager::GetOperation:
        if(!dataQuery.isEmpty()) {
            requestUrl.setQuery(dataQuery);
        }
        request.setUrl(requestUrl);
        reply = manager.get(request);
        break;
    case QNetworkAccessManager::PostOperation:
        request.setUrl(requestUrl);
        reply = manager.post(request, dataQuery.toString(QUrl::FullyEncoded).toUtf8());
        break;
    default:
        break;
    }

    connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
    connect(&manager, SIGNAL(finished(QNetworkReply*)), &loop, SLOT(quit()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), &loop, SLOT(quit()));
    timer.start(15000);
    loop.exec();

    error = reply->error();
    errorString = reply->errorString();
    response = reply->readAll();
    reply->deleteLater();

    if(timer.isActive()) {
        if(error == QNetworkReply::NoError) {
            return response;
        } else {
            if(response.isEmpty()) {
                throw std::runtime_error(errorString.toStdString());
            } else {
                throw std::runtime_error(QJsonDocument::fromJson(response).object().value("errors").toArray().first().toObject().value("message").toString().toStdString());
            }
        }
    } else {
        throw std::runtime_error(std::string(tr("タイムアウトしました。").toStdString()));
    }
    return NULL;
}

UsersObject RestClient::verifyCredentials()
{
    try {
        return UsersObject(requestTwitterApi(QNetworkAccessManager::GetOperation, ACCOUNT_VERIFY_CREDENTIALS_URL));
    }catch (...) {
        throw;
    }
}

void RestClient::statusUpdate(const QString &text, const QString &in_reply_to_status_id, const QStringList &media_ids)
{
    QVariantMap dataParams;
    dataParams["status"] = text;
    if(!in_reply_to_status_id.isEmpty()) {
        dataParams["in_reply_to_status_id"] = in_reply_to_status_id;
    }
    if(!media_ids.isEmpty()) {
        dataParams["media_ids"] = media_ids.join(",");
    }
    try {
        requestTwitterApi(QNetworkAccessManager::PostOperation, STATUSES_UPDATE_URL, dataParams);
    } catch(...) {
        throw;
    }
}

UsersObject RestClient::updateName(const QString &name)
{
    try {
        return updateProfile(name);
    } catch(...) {
        throw;
    }
}

UsersObject RestClient::updateUrl(const QString &url)
{
    try {
        return updateProfile(NULL, url);
    } catch(...) {
        throw;
    }
}

UsersObject RestClient::updateLocation(const QString &location)
{
    try {
        return updateProfile(NULL, NULL, location);
    } catch(...) {
        throw;
    }
}

UsersObject RestClient::updateDescroption(const QString &description)
{
    try {
        return updateProfile(NULL, NULL, NULL, description);
    } catch(...) {
        throw;
    }
}

UsersObject RestClient::updateProfile(const QString &name, const QString &url, const QString &location, const QString &description)
{
    QVariantMap dataParams;
    if(!name.isEmpty()) {
        dataParams["name"] = name;
    }
    if(!url.isEmpty()) {
        dataParams["url"] = url;
    }
    if(!location.isEmpty()) {
        dataParams["location"] = location;
    }
    if(!description.isEmpty()) {
        dataParams["description"] = description;
    }
    if(dataParams.isEmpty()) {
        return UsersObject(NULL);
    }
    try {
        return UsersObject(requestTwitterApi(QNetworkAccessManager::PostOperation, ACCOUNT_UPDATE_PROFILE_URL, dataParams));
    } catch(...) {
        throw;
    }
}

UsersObject RestClient::usersLookup(const QString &screenName, const QString &userId)
{
    QVariantMap dataParams;
    if(!screenName.isEmpty()) {
        qDebug("screen");
        dataParams["screen_name"] = screenName;
    }
    if(!userId.isEmpty()) {
        dataParams["user_id"] = userId;
    }
    if(dataParams.isEmpty()) {
        return UsersObject(NULL);
    }
    try {
        return UsersObject(requestTwitterApi(QNetworkAccessManager::GetOperation, USERS_LOOKUP_URL, dataParams));
    } catch(...) {
        throw;
    }
}

QString RestClient::mediaUpload(const QString &mediaFileName)
{
    QVariantMap dataParams;
    QFile mediaFile(mediaFileName);
    if(mediaFile.open(QFile::ReadOnly)) {
        dataParams["media"] = mediaFile.readAll().toBase64();
        mediaFile.close();
        try {
            return QJsonDocument::fromJson(requestTwitterApi(QNetworkAccessManager::PostOperation,
                                                             MEDIA_UPLOAD_URL,
                                                             dataParams)).object().value("media_id_string").toString();
        }catch(...) {
            throw;
        }
    } else {
        throw std::runtime_error(mediaFile.errorString().toStdString());
    }
}

QString RestClient::mediaUpload(const QByteArray &mediaData)
{
    QVariantMap dataParams;
    dataParams["media"] = mediaData.toBase64();
    try {
        return QJsonDocument::fromJson(requestTwitterApi(QNetworkAccessManager::PostOperation,
                                                         MEDIA_UPLOAD_URL,
                                                         dataParams)).object().value("media_id_string").toString();
    } catch(...) {
        throw;
    }
}

UsersObject RestClient::updateProfileImage(const QByteArray &mediaData)
{
    QVariantMap dataParams;
    dataParams["image"] = mediaData.toBase64();
    try {
        return UsersObject(requestTwitterApi(QNetworkAccessManager::PostOperation,
                                             ACCOUT_UPDATE_PROFILE_IMAGE_URL,
                                             dataParams));
    } catch(...) {
        throw;
    }
}

UsersObject RestClient::updateProfileBackground(const QByteArray &mediaData)
{
    QVariantMap dataParams;
    dataParams["image"] = mediaData.toBase64();
    dataParams["use"] = true;
    try {
        return UsersObject(requestTwitterApi(QNetworkAccessManager::PostOperation,
                                             ACCOUNT_UPDATE_PROFILE_BACKGROUND_IMAGE_URL,
                                             dataParams));
    } catch(...) {
        throw;
    }
}

UsersObject RestClient::updateProfileBanner(const QByteArray &mediaData)
{
    QVariantMap dataParams;
    dataParams["banner"] = mediaData.toBase64();
    try {
        return UsersObject(requestTwitterApi(QNetworkAccessManager::PostOperation,
                                             ACCOUNT_UPDATE_PROFILE_BANNER_URL,
                                             dataParams));
    } catch (...) {
        throw;
    }
}
