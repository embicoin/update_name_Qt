#ifndef ERROROBJECT_H
#define ERROROBJECT_H

#include "object.h"

namespace TwitterAPI {
namespace Object {
class Error : public TwitterAPI::Object::Object
{
public:
    Error(const QByteArray &json = NULL);
    int code() const;
    QString message() const;
};
}
}

#endif // ERROROBJECT_H
