#include "update.h"

#include <QDebug>

using namespace TwitterAPI::Rest::Statuses;

UpdateParameters::UpdateParameters()
{
    qRegisterMetaType<TwitterAPI::Rest::Statuses::UpdateParameters>("TwitterAPI::Rest::Statuses::UpdateParameters");
}

const QUrl Update::UPDATE_URL("https://api.twitter.com/1.1/statuses/update.json");

Update::Update(QObject *parent) :
    RestApi(parent)
{
    connect(this, SIGNAL(apiError(ErrorObject)), this, SIGNAL(finished()));
    connect(this, SIGNAL(networkError(QString)), this, SIGNAL(finished()));
    connect(this, SIGNAL(successed(TweetObject)), this, SIGNAL(finished()));
}

Update::Update(const TwitterAPI::OAuth &oauth, QObject *parent) :
    RestApi(oauth, parent)
{
    connect(this, SIGNAL(apiError(ErrorObject)), this, SIGNAL(finished()));
    connect(this, SIGNAL(networkError(QString)), this, SIGNAL(finished()));
    connect(this, SIGNAL(successed(TweetObject)), this, SIGNAL(finished()));
}

TwitterAPI::Object::Tweets Update::exec(const QString &status, const QString &inReplyToStatusId,
                                        const bool possiblySensitive, const QString &lat, const QString &_long,
                                        const QString &placeId, const bool &displayCoordinates, const QStringList &mediaIds)
{
    QVariantMap params;
    params["status"] = status;
    if (inReplyToStatusId.isEmpty())
        params["in_reply_to_status_id"] = inReplyToStatusId;
    if (possiblySensitive)
        params["possibly_sensitive"] = possiblySensitive;
    if (!lat.isEmpty())
        params["lat"] = lat;
    if (!_long.isEmpty())
        params["long"] = _long;
    if (!placeId.isEmpty())
        params["place_id"] = placeId;
    if (!displayCoordinates)
        params["display_coordinates"] = displayCoordinates;
    if (!mediaIds.isEmpty()) {
        QString ids;
        for (int i = 0; i < mediaIds.size() && i < 4; i++)
            ids.append(mediaIds[i] + ",");
        ids.chop(1);
        params["media_ids"] = ids;
    }
    try {
        TwitterAPI::Object::Tweets tweet(requestApi(QNetworkAccessManager::PostOperation, UPDATE_URL, params));
        emit successed(tweet);
        return tweet;
    } catch (const std::runtime_error &e) {
        emit networkError(QString::fromStdString(e.what()));
    } catch (const TwitterAPI::Object::Error &e) {
        emit apiError(e);
    }

    return TwitterAPI::Object::Tweets(NULL);
}

TwitterAPI::Object::Tweets Update::exec(const UpdateParameters &parameters)
{
    QVariantMap params;
    params["status"] = parameters.status;
    if (!parameters.inReplyToStatusId.isNull())
        params["in_reply_to_status_id"] = parameters.inReplyToStatusId;
    if (parameters.possiblySensitive)
        params["possibly_sensitive"] = parameters.possiblySensitive;
    if (!parameters.lat.isNull())
        params["lat"] = parameters.lat;
    if (!parameters._long.isNull())
        params["long"] = parameters._long;
    if (!parameters.placeId.isNull())
        params["place_id"] = parameters.placeId;
    if (!parameters.displayCoordinates)
        params["display_coordinates"] = parameters.displayCoordinates;
    if (!parameters.mediaIds.isEmpty()) {
        QString ids;
        for (int i = 0; i < parameters.mediaIds.size() && i < 4; i++)
            ids.append(parameters.mediaIds[i] + ",");
        ids.chop(1);
        params["media_ids"] = ids;
    }
    try {
        TwitterAPI::Object::Tweets tweet(requestApi(QNetworkAccessManager::PostOperation, UPDATE_URL, params));
        emit successed(tweet);
        return tweet;
    } catch (const std::runtime_error &e) {
        emit networkError(QString::fromStdString(e.what()));
    } catch (const TwitterAPI::Object::Error &e) {
        emit apiError(e);
    }

    return TwitterAPI::Object::Tweets(NULL);
}
