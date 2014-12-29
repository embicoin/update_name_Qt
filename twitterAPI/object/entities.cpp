#include "entities.h"

#include <QJsonDocument>
#include <QMetaType>
#include <QJsonArray>
#include <QVariant>

using namespace TwitterAPI::Object;

Size::Size(const QByteArray &json)
    : Object(json)
{
   qRegisterMetaType<TwitterAPI::Object::Size>("TwitterAPI::Object::Size");
}

int Size::h() const
{
    return m_object.value("h").toInt();
}

QString Size::resize() const
{
    return m_object.value("resize").toString();
}

int Size::w() const
{
    return m_object.value("w").toInt();
}

Sizes::Sizes(const QByteArray &json)
    : Object(json)
{
    qRegisterMetaType<TwitterAPI::Object::Sizes>("TwitterAPI::Object::Sizes");
}

Size Sizes::thumb() const
{
    QJsonDocument doc;
    doc.setObject(m_object.value("thumb").toObject());
    return doc.toJson();
}

Size Sizes::large() const
{
    QJsonDocument doc;
    doc.setObject(m_object.value("large").toObject());
    return doc.toJson();
}

Size Sizes::medium() const
{
    QJsonDocument doc;
    doc.setObject(m_object.value("medium").toObject());
    return doc.toJson();
}

Size Sizes::small() const
{
    QJsonDocument doc;
    doc.setObject(m_object.value("small").toObject());
    return doc.toJson();
}

HashTags::HashTags(const QByteArray &json)
    : Object(json)
{
    qRegisterMetaType<TwitterAPI::Object::HashTags>("TwitterAPI::Object::HashTags");
}

QList<int> HashTags::indices() const
{
    QList<int> ind;
    QJsonArray array = m_object.value("indices").toArray();

    for (auto i = array.constBegin(); i != array.constEnd(); i++)
        ind << (*i).toInt();

    return ind;
}

QString HashTags::text() const
{
    return m_object.value("text").toString();
}

Media::Media(const QByteArray &json)
    : Object(json)
{
    qRegisterMetaType<TwitterAPI::Object::Media>("TwitterAPI::Object::Media");
}

QString Media::displayUrl() const
{
    return m_object.value("display_url").toString();
}

QUrl Media::expandedUrl() const
{
    return QUrl(m_object.value("expanded_url").toString());
}

qint64 Media::id() const
{
    return m_object.value("id").toVariant().toLongLong();
}

QString Media::idStr() const
{
    return m_object.value("id_str").toString();
}

QList<int> Media::indices() const
{
    QList<int> ind;
    QJsonArray array = m_object.value("indices").toArray();

    for (auto i = array.constBegin(); i != array.constEnd(); i++)
        ind << (*i).toInt();

    return ind;
}

QUrl Media::mediaUrl() const
{
    return QUrl(m_object.value("media_url").toString());
}

QUrl Media::mediaUrlHttps() const
{
    return QUrl(m_object.value("media_url_https").toString());
}

Sizes Media::sizes() const
{
    QJsonDocument doc;
    doc.setObject(m_object.value("sizes").toObject());
    return doc.toJson();
}

qint64 Media::sourceStatusId() const
{
    return m_object.value("source_status_id").toVariant().toLongLong();
}

QString Media::sourceStatusIdStr() const
{
    return m_object.value("source_status_id_str").toString();
}

QString Media::type() const
{
    return m_object.value("type").toString();
}

QUrl Media::url() const
{
    return QUrl(m_object.value("url").toString());
}

Url::Url(const QByteArray &json)
    : Object(json)
{
    qRegisterMetaType<TwitterAPI::Object::Url>("TwitterAPI::Object::Url");
}

QString Url::displayUrl() const
{
    return m_object.value("display_url").toString();
}

QUrl Url::expandedUrl() const
{
    return QUrl(m_object.value("expanded_url").toString());
}

QList<int> Url::indices() const
{
    QList<int> ind;
    QJsonArray array = m_object.value("indices").toArray();

    for (auto i = array.constBegin(); i != array.constEnd(); i++)
        ind << (*i).toInt();

    return ind;
}

QUrl Url::url() const
{
    return QUrl(m_object.value("url").toString());
}

UserMention::UserMention(const QByteArray &json)
    : Object(json)
{
    qRegisterMetaType<TwitterAPI::Object::UserMention>("TwitterAPI::Object::UserMention");
}

qint64 UserMention::id() const
{
    return m_object.value("id").toVariant().toLongLong();
}

QString UserMention::idStr() const
{
    return m_object.value("id_str").toString();
}

QList<int> UserMention::indices() const
{
    QList<int> ind;
    QJsonArray array = m_object.value("indices").toArray();

    for (auto i = array.constBegin(); i != array.constEnd(); i++)
        ind << (*i).toInt();

    return ind;
}

QString UserMention::name() const
{
    return m_object.value("name").toString();
}

QString UserMention::screenName() const
{
    return m_object.value("screen_name").toString();
}

Entities::Entities(const QByteArray &json)
    : Object(json)
{
    qRegisterMetaType<TwitterAPI::Object::Entities>("TwitterAPI::Object::Entities");
}

QList<HashTags> Entities::hashtags() const
{
    QList<HashTags> tags;
    QJsonArray array = m_object.value("hashtags").toArray();

    for (auto i = array.constBegin(); i != array.constEnd(); i++) {
        QJsonDocument doc;
        doc.setObject((*i).toObject());
        tags << HashTags(doc.toJson());
    }

    return tags;
}

QList<Media> Entities::media() const
{
    QList<Media> medias;
    QJsonArray array = m_object.value("media").toArray();

    for (auto i = array.constBegin(); i != array.constEnd(); i++) {
        QJsonDocument doc;
        doc.setObject((*i).toObject());
        medias << Media(doc.toJson());
    }

    return medias;
}

QList<Url> Entities::urls() const
{
    QList<Url> urls;
    QJsonArray array = m_object.value("urls").toArray();

    for (auto i = array.constBegin(); i != array.constEnd(); i++) {
        QJsonDocument doc;
        doc.setObject((*i).toObject());
        urls << Url(doc.toJson());
    }

    return urls;
}

QList<UserMention> Entities::userMentions() const
{
    QList<UserMention> mentions;
    QJsonArray array = m_object.value("user_mentions").toArray();

    for (auto i = array.constBegin(); i != array.constEnd(); i++) {
        QJsonDocument doc;
        doc.setObject((*i).toObject());
        mentions << UserMention(doc.toJson());
    }

    return mentions;
}
