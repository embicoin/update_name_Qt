#include "upload.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>

using namespace TwitterAPI::Rest::Media;
using TwitterAPI::OAuth;

const QUrl Upload::UPLOAD_URL("https://upload.twitter.com/1.1/media/upload.json");

Upload::Upload(QObject *parent) :
    RestApi(parent)
{
    connect(this, SIGNAL(apiError(ErrorObject)), this, SIGNAL(finished()));
    connect(this, SIGNAL(fileError(QString)), this, SIGNAL(finished()));
    connect(this, SIGNAL(networkError(QString)), this, SIGNAL(finished()));
    connect(this, SIGNAL(successed(QString)), this, SIGNAL(finished()));
}

Upload::Upload(const OAuth &oauth, QObject *parent) :
    RestApi(oauth, parent)
{
    connect(this, SIGNAL(apiError(ErrorObject)), this, SIGNAL(finished()));
    connect(this, SIGNAL(fileError(QString)), this, SIGNAL(finished()));
    connect(this, SIGNAL(networkError(QString)), this, SIGNAL(finished()));
    connect(this, SIGNAL(successed(QString)), this, SIGNAL(finished()));
}

QString Upload::exec(const QFile &mediaFile)
{
    QFile file(mediaFile.fileName());
    QVariantMap params;
    if (file.open(QFile::ReadOnly)) {
        params["media"] = file.readAll().toBase64();
        try {
            QString mediaId = QJsonDocument::fromJson(requestApi(QNetworkAccessManager::PostOperation, UPLOAD_URL, params))
                    .object().value("image_id_string").toString();
            emit successed(mediaId);
            return mediaId;
        } catch (const std::runtime_error &e) {
            emit networkError(QString::fromStdString(e.what()));
        } catch (const TwitterAPI::Object::Error &e) {
            emit apiError(e);
        }
    } else {
        emit fileError(file.errorString());
    }
    return QString::null;
}

QString Upload::exec(const QString &filePath)
{
    QFile file(filePath);
    QVariantMap params;
    if (file.open(QFile::ReadOnly)) {
        params["media"] = file.readAll().toBase64();
        try {
            QString id = QJsonDocument::fromJson(requestApi(QNetworkAccessManager::PostOperation, UPLOAD_URL, params))
                    .object().value("media_id_string").toString();
            emit successed(id);
            return id;
        } catch (const std::runtime_error &e) {
            emit networkError(QString::fromStdString(e.what()));
        } catch (const TwitterAPI::Object::Error &e) {
            emit apiError(e);
        }
    } else {
        emit fileError(file.errorString());
    }
    return NULL;
}
