#include "tweetobject.h"

#include <QJsonDocument>

#include <QDebug>

TweetObject::TweetObject(const QByteArray &json)
{
    m_object = QJsonDocument::fromJson(json).object();
}

QString TweetObject::idStr() const
{
    return m_object.value("id_str").toString();
}

QString TweetObject::text() const
{
    return m_object.value("text").toString();
}

UsersObject TweetObject::user() const
{
    QJsonDocument doc;
    doc.setObject(m_object.value("user").toObject());
    return UsersObject(doc.toJson());
}
