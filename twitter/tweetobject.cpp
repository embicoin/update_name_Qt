#include "tweetobject.h"

#include <QJsonDocument>

#include <QDebug>

TweetObject::TweetObject(const QByteArray &json)
{
    object = QJsonDocument::fromJson(json).object();
}

QString TweetObject::idStr() const
{
    return object.value("id_str").toString();
}

QString TweetObject::text() const
{
    return object.value("text").toString();
}

UsersObject TweetObject::user() const
{
    QJsonDocument doc;
    doc.setObject(object.value("user").toObject());
    return UsersObject(doc.toJson());
}
