/*
Copyright 2014 owata_programer

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include "tweets.h"

#include <QJsonDocument>
#include <QJsonValue>
#include <QVariant>
#include <QJsonArray>

using TwitterAPI::Object::Tweets;

Tweets::Tweets(const QByteArray &json)
{
    m_object = QJsonDocument::fromJson(json).object();
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
