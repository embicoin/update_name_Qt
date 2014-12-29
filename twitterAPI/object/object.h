#ifndef OBJECT_H
#define OBJECT_H

#include <QJsonObject>

namespace TwitterAPI {
namespace Object {
class Object
{
public:
    void setJson(const QByteArray &json);

protected:
    Object(const QByteArray &json = NULL);
    QJsonObject m_object;
};
}
}

#endif // OBJECT_H
