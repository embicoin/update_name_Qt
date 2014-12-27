#include "parsejson.h"

#include <QVariant>
#include <QDebug>

using namespace TwitterAPI::Streaming;

StatusDeletionNotices::StatusDeletionNotices()
{
    qRegisterMetaType<TwitterAPI::Streaming::StatusDeletionNotices>("TwitterAPI::Streaming::StatusDeletionNotices");
}

ParseJson::ParseJson(const QByteArray &json, QObject *parent)
    : QThread(parent)
{
    QJsonParseError e;
    m_json = QJsonDocument::fromJson(json, &e);
    if (e.error != QJsonParseError::NoError)
        qDebug() << e.errorString() << json;
}

void ParseJson::run()
{
    const QJsonObject object = m_json.object();
    if (!object.value("text").isUndefined()) {
        qDebug() << TwitterAPI::Object::Tweets(m_json.toJson()).text();
        emit tweet(TwitterAPI::Object::Tweets(m_json.toJson()));
    } else if (!object.value("delete").isUndefined()) {
        StatusDeletionNotices statusDeletionNotices;
        const QJsonObject statusObject = object.value("status").toObject();
        statusDeletionNotices.id = statusObject.value("id").toVariant().toLongLong();
        statusDeletionNotices.idStr = statusObject.value("id_str").toString();
        statusDeletionNotices.userId = statusObject.value("user_id").toVariant().toLongLong();
        statusDeletionNotices.userIdStr = statusObject.value("user_id_str").toString();
        emit statusDeletion(statusDeletionNotices);
    }
}
