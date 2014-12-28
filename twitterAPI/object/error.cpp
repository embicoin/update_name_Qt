#include "error.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QMetaType>

using namespace TwitterAPI::Object;

Error::Error(const QByteArray &json)
{
    qRegisterMetaType<TwitterAPI::Object::Error>("TwitterAPI::Object::Error");
    m_object = QJsonDocument::fromJson(json).object();
}

void Error::setJson(const QByteArray &json)
{
    m_object = QJsonDocument::fromJson(json).object();
}

int Error::code() const
{
    return m_object.value("errors").toArray().first().toObject().value("code").toInt();
}

QString Error::message() const
{
    return m_object.value("errors").toArray().first().toObject().value("message").toString();
}
