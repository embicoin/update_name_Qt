#ifndef TWEETOBJECT_H
#define TWEETOBJECT_H

#include "usersobject.h"

#include <QByteArray>
#include <QJsonObject>

class TweetObject
{
public:
    TweetObject(const QByteArray &json);

    QString idStr() const;
    QString text() const;
    UsersObject user() const;

private:
    QJsonObject m_object;
};

#endif // TWEETOBJECT_H
