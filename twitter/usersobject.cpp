#include "usersobject.h"

#include <QJsonDocument>

UsersObject::UsersObject(const QByteArray &json)
{
    m_object = QJsonDocument::fromJson(json).object();
}

QString UsersObject::name() const
{
    return m_object.value("name").toString();
}

QString UsersObject::screen_name() const
{
    return m_object.value("screen_name").toString();
}

QUrl UsersObject::url() const
{
    return QUrl(m_object.value("url").toString());
}

QString UsersObject::location() const
{
    return m_object.value("location").toString();
}

QString UsersObject::description() const
{
    return m_object.value("description").toString();
}

QUrl UsersObject::profileImageUrl() const
{
    return QUrl(m_object.value("profile_image_url").toString());
}

QUrl UsersObject::profileImageUrlHttps() const
{
    return QUrl(m_object.value("profile_image_url_https").toString());
}
