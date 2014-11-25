#include "entitiesobject.h"

#include <QJsonDocument>
#include <QJsonArray>

EntitiesObject::MediaObject::MediaObject(const QByteArray &json)
{
    m_object = QJsonDocument::fromJson(json).object();
}

QString EntitiesObject::MediaObject::displayUrl() const
{
    return m_object.value("display_url").toString();
}

QUrl EntitiesObject::MediaObject::expandedUrl() const
{
    return QUrl(m_object.value("expanded_url").toString());
}

QString EntitiesObject::MediaObject::idStr() const
{
    return m_object.value("id_str").toString();
}

QUrl EntitiesObject::MediaObject::mediaUrl() const
{
    return QUrl(m_object.value("media_url").toString());
}

QUrl EntitiesObject::MediaObject::mediaUrlHttps() const
{
    return QUrl(m_object.value("media_url_https").toString());
}

EntitiesObject::EntitiesObject(const QByteArray &json)
{
    m_object = QJsonDocument::fromJson(json).object();
}

EntitiesObject::MediaObject EntitiesObject::media() const
{
    QJsonDocument doc;
    doc.setObject(m_object.value("media").toArray().first().toObject());
    return MediaObject(doc.toJson());
}
