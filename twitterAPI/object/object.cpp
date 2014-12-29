#include "object.h"

#include <QJsonDocument>

using namespace TwitterAPI::Object;

Object::Object(const QByteArray &json)
{
    setJson(json);
}

void Object::setJson(const QByteArray &json)
{
    m_object = QJsonDocument::fromJson(json).object();
}

