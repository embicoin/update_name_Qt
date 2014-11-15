#include "twitter.h"
#include "update_name_oauth.h"

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

const QString Twitter::ACCOUNT_VERIFY_CREDENTIALS_URL = "https://api.twitter.com/1.1/account/verify_credentials.json";
const QString Twitter::ACCOUNT_UPDATE_PROFILE_URL     = "https://api.twitter.com/1.1/account/update_profile.json";
const QString Twitter::STATUSES_UPDATE_URL            = "https://api.twitter.com/1.1/statuses/update.json";
const QString Twitter::MEDIA_UPLOAD_URL               = "https://upload.twitter.com/1.1/media/upload.json";

Twitter::Twitter(QObject *parent) :
    QObject(parent)
{
}

QByteArray Twitter::requestTwitterApi(const QNetworkAccessManager::Operation method,
                                      const QString &url,
                                      const QVariantMap &data_params,
                                      const QString &content_type)
{
    const QByteArray signature_key = settings.consumerSecret().toUtf8()
            + "&"
            + settings.accessTokenSecret().toUtf8();
    QByteArray http_method_string;
    QVariantMap oauth_params;
    QVariantMap signature_params;
    QUrlQuery signature_query;
    QByteArray signature_base_string;
    QByteArray oauth_header;
    QUrlQuery data_query;
    QUrl request_url(url);
    QNetworkRequest request;
    QNetworkAccessManager manager;
    QNetworkReply *reply;
    QTimer timer;
    QEventLoop loop;
    QNetworkReply::NetworkError error;
    QString error_string;
    QByteArray response;

    switch (method) {
    case QNetworkAccessManager::GetOperation:
        http_method_string = "GET";
        break;
    case QNetworkAccessManager::PostOperation:
        http_method_string = "POST";
        break;
    default:
        return NULL;
        break;
    }

    oauth_params["oauth_consumer_key"]     = settings.consumerKey();
    oauth_params["oauth_token"]            = settings.accessToken();
    oauth_params["oauth_signature_method"] = OAUTH_SIGNATURE_METHOD;
    oauth_params["oauth_timestamp"]        = OAUTH_TIMESTAMP;
    oauth_params["oauth_nonce"]            = OAUTH_NONCE;
    oauth_params["oauth_version"]          = OAUTH_VERSION;

    for(int i = 0; i < oauth_params.size(); i++) {
        signature_params.insert(oauth_params.keys()[i], oauth_params.values()[i]);
    }

    if(!data_params.isEmpty()) {
        for(int i = 0; i < data_params.size(); i++) {
            signature_params.insert(data_params.keys()[i], data_params.values()[i].toByteArray().toPercentEncoding());
            data_query.addQueryItem(data_params.keys()[i], data_params.values()[i].toByteArray().toPercentEncoding());
        }
    }

    for(int i = 0; i < signature_params.size(); i++) {
        signature_query.addQueryItem(signature_params.keys()[i], signature_params.values()[i].toString());
    }

    signature_base_string = http_method_string
            + "&"
            + url.toUtf8().toPercentEncoding()
            + "&"
            + signature_query.toString(QUrl::FullyEncoded).toUtf8().toPercentEncoding();

    oauth_params["oauth_signature"] = QMessageAuthenticationCode::hash(signature_base_string,
                                                                      signature_key,
                                                                      QCryptographicHash::Sha1).toBase64().toPercentEncoding();

    oauth_header.append("OAuth ");
    for(int i = 0; i < oauth_params.size(); i++) {
        oauth_header.append(oauth_params.keys()[i])
                   .append("=\"")
                   .append(oauth_params.values()[i].toByteArray())
                   .append("\", ");
    }
    oauth_header.remove(oauth_header.length() - 2, 2);

    request.setHeader(QNetworkRequest::ContentTypeHeader, content_type);
    request.setRawHeader("Authorization", oauth_header);

    switch (method) {
    case QNetworkAccessManager::GetOperation:
        if(!data_query.isEmpty()) {
            request_url.setQuery(data_query);
        }
        request.setUrl(request_url);
        reply = manager.get(request);
        break;
    case QNetworkAccessManager::PostOperation:
        request.setUrl(request_url);
        reply = manager.post(request, data_query.toString(QUrl::FullyEncoded).toUtf8());
        break;
    default:
        return NULL;
        break;
    }

    connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
    connect(&manager, SIGNAL(finished(QNetworkReply*)), &loop, SLOT(quit()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), &loop, SLOT(quit()));
    timer.start(15000);
    loop.exec();

    error = reply->error();
    error_string = reply->errorString();
    response = reply->readAll();
    reply->deleteLater();

    if(timer.isActive()) {
        if(error == QNetworkReply::NoError) {
            return response;
        } else {
            if(response.isEmpty()) {
                throw std::runtime_error(error_string.toStdString());
            } else {
                throw std::runtime_error(QJsonDocument::fromJson(response).object().value("errors").toArray().first().toObject().value("message").toString().toStdString());
            }
        }
    } else {
        throw std::runtime_error(std::string(tr("タイムアウトしました。").toStdString()));
    }
    return NULL;
}

QString Twitter::getScreenName()
{
    QByteArray response;
    try {
        response = requestTwitterApi(QNetworkAccessManager::GetOperation, ACCOUNT_VERIFY_CREDENTIALS_URL);
    } catch (...) {
        throw;
        return NULL;
    }
    return QJsonDocument::fromJson(response).object().value("screen_name").toString();
}

QString Twitter::getName()
{
    QByteArray response;
    try {
        response = requestTwitterApi(QNetworkAccessManager::GetOperation, ACCOUNT_VERIFY_CREDENTIALS_URL);
    } catch(...) {
        throw;
        return NULL;
    }
    return QJsonDocument::fromJson(response).object().value("name").toString();
}

void Twitter::statusUpdate(const QString &text, const QString &in_reply_to_status_id, const QStringList &media_ids)
{
    QVariantMap data_params;
    data_params["status"] = text;
    if(!in_reply_to_status_id.isEmpty()) {
        data_params["in_reply_to_status_id"] = in_reply_to_status_id;
    }
    if(!media_ids.isEmpty()) {
        data_params["media_ids"] = media_ids.join(",");
    }
    try {
        requestTwitterApi(QNetworkAccessManager::PostOperation, STATUSES_UPDATE_URL, data_params);
    } catch(...) {
        throw;
    }
}

void Twitter::updateName(const QString &name)
{
    QVariantMap data_params;
    data_params["name"] = name;
    try {
        requestTwitterApi(QNetworkAccessManager::PostOperation, ACCOUNT_UPDATE_PROFILE_URL, data_params);
    } catch(...) {
        throw;
    }
}

void Twitter::udpateProfile(const QString &name, const QString &url, const QString &location, const QString &description)
{
    QVariantMap data_params;
    if(!name.isEmpty()) {
        data_params["name"] = name;
    }
    if(!url.isEmpty()) {
        data_params["url"] = url;
    }
    if(!location.isEmpty()) {
        data_params["location"] = location;
    }
    if(!description.isEmpty()) {
        data_params["description"] = description;
    }
    if(data_params.isEmpty()) {
        return;
    }
    try {
        requestTwitterApi(QNetworkAccessManager::PostOperation, ACCOUNT_UPDATE_PROFILE_URL, data_params);
    } catch(...) {
        throw;
    }
}

QString Twitter::mediaUpload(const QString &media_file_name)
{
    QVariantMap data_params;
    QFile media_file(media_file_name);
    media_file.open(QFile::ReadOnly);
    data_params["media"] = media_file.readAll().toBase64();
    media_file.close();
    try {
        return requestTwitterApi(QNetworkAccessManager::PostOperation,
                                 MEDIA_UPLOAD_URL,
                                 data_params);
    }catch(...) {
        throw;
    }
}
