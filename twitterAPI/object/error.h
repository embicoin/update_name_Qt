#ifndef ERROROBJECT_H
#define ERROROBJECT_H

#include <QJsonObject>

namespace TwitterAPI {
namespace Object {
class Error
{
public:
    Error(const QByteArray &json = NULL);
    void setJson(const QByteArray &json);

    int code() const;
    QString message() const;

private:
    QJsonObject m_object;
};
}
}

#endif // ERROROBJECT_H
