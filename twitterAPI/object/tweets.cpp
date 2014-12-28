#include "tweets.h"

#include <QJsonDocument>
#include <QJsonValue>
#include <QVariant>
#include <QJsonArray>

using namespace TwitterAPI::Object;

Tweets::Tweets(const QByteArray &json)
{
    m_object = QJsonDocument::fromJson(json).object();
    qRegisterMetaType<TwitterAPI::Object::Tweets>("TwitterAPI::Object::Tweets");
}

void Tweets::setJson(const QByteArray &json)
{
    m_object = QJsonDocument::fromJson(json).object();
}

QString Tweets::createdAt() const
{
    return m_object.value("created_at").toString();
}

int Tweets::favoriteCount() const
{
    return m_object.value("favorite_count").toInt();
}

TwitterAPI::triBool Tweets::favorited() const
{
    QJsonValue v = m_object.value("favorited");
    if (v.isNull()) {
        return TwitterAPI::Null;
    } else {
        if (v.toBool())
            return TwitterAPI::True;
        else
            return TwitterAPI::False;
    }
}

QString Tweets::filterLevel() const
{
    return m_object.value("filter_level").toString();
}

QString Tweets::idStr() const
{
    return m_object.value("id_str").toString();
}

QString Tweets::inReplyToScreenName() const
{
    return m_object.value("in_reply_to_screen_name").toString();
}

QString Tweets::inReplyToStatusIdStr() const
{
    return m_object.value("in_reply_to_status_id_str").toString();
}

QString Tweets::inReplyToUserIdStr() const
{
    return m_object.value("in_reply_to_user_id_str").toString();
}

QString Tweets::lang() const
{
    return m_object.value("lang").toString();
}

TwitterAPI::triBool Tweets::possiblySensitive() const
{
    QJsonValue v = m_object.value("possibly_sensitive");
    if (v.isNull()) {
        return TwitterAPI::Null;
    } else {
        if (v.toBool())
            return TwitterAPI::True;
        else
            return TwitterAPI::False;
    }
}

int Tweets::retweetCount() const
{
    return m_object.value("retweet_count").toInt();
}

bool Tweets::retweeted() const
{
    return m_object.value("retweeted").toBool();
}

QString Tweets::source() const
{
    return m_object.value("source").toString();
}

QString Tweets::text() const
{
    return m_object.value("text").toString();
}

bool Tweets::truncated() const
{
    return m_object.value("truncated").toBool();
}

Users Tweets::user() const
{
    QJsonDocument doc;
    doc.setObject(m_object.value("user").toObject());
    return Users(doc.toJson());
}

bool Tweets::withheldCopyright() const
{
    return m_object.value("withheld_copyright").toBool();
}

QStringList Tweets::withheldInCountries() const
{
    QVariantList countries = m_object.value("withheld_in_countries").toArray().toVariantList();
    QStringList array;

    for(QVariantList::const_iterator i = countries.constBegin(); i != countries.constEnd(); i++)
        array << (*i).toString();
    return array;
}

QString Tweets::withheldScope() const
{
    return m_object.value("withheld_scope").toString();
}
