#include "usersobject.h"

#include <QJsonDocument>

UsersObject::UsersObject(const QByteArray &json)
{
    object = QJsonDocument::fromJson(json).object();
}

QString UsersObject::name() const
{
    return object.value("name").toString();
}

QString UsersObject::screen_name() const
{
    return object.value("screen_name").toString();
}
