#include "parsejson.h"

#include <QVariant>

using namespace TwitterAPI::Streaming;

StatusDeletionNotices::StatusDeletionNotices()
{
    qRegisterMetaType<TwitterAPI::Streaming::StatusDeletionNotices>("TwitterAPI::Streaming::StatusDeletionNotices");
}

ParseJson::ParseJson(const QByteArray &json, QObject *parent)
    : QThread(parent)
{
    m_json = QJsonDocument::fromJson(json);
}

void ParseJson::run()
{
    const QJsonObject object = m_json.object();
    if (!object.value("in_reply_to_status_id").isNull()) {
        emit tweet(TwitterAPI::Object::Tweets(m_json.toJson()));
    } else if (!object.value("delete").isNull()) {
        StatusDeletionNotices statusDeletionNotices;
        const QJsonObject statusObject = object.value("status").toObject();
        statusDeletionNotices.id = statusObject.value("id").toVariant().toLongLong();
        statusDeletionNotices.idStr = statusObject.value("id_str").toString();
        statusDeletionNotices.userId = statusObject.value("user_id").toVariant().toLongLong();
        statusDeletionNotices.userIdStr = statusObject.value("user_id_str").toString();
        emit statusDeletion(statusDeletionNotices);
    }
}
