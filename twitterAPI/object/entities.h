#ifndef ENTITIES_H
#define ENTITIES_H

#include "../twitterglobal.h"
#include "object.h"

#include <QJsonObject>
#include <QUrl>

namespace TwitterAPI {
namespace Object {
class Size : public TwitterAPI::Object::Object
{
public:
    Size(const QByteArray &json = NULL);

    int h() const;
    QString resize() const;
    int w() const;
};

class Sizes : public TwitterAPI::Object::Object
{
public:
    Sizes(const QByteArray &json = NULL);

    Size thumb() const;
    Size large() const;
    Size medium() const;
    Size small() const;
};

class HashTags : public TwitterAPI::Object::Object
{
public:
    HashTags(const QByteArray &json = NULL);

    QList<int> indices() const;
    QString text() const;
};

class Media : public TwitterAPI::Object::Object
{
public:
    Media(const QByteArray &json = NULL);

    QString displayUrl() const;
    QUrl expandedUrl() const;
    qint64 id() const;
    QString idStr() const;
    QList<int> indices() const;
    QUrl mediaUrl() const;
    QUrl mediaUrlHttps() const;
    Sizes sizes() const;
    qint64 sourceStatusId() const;
    QString sourceStatusIdStr() const;
    QString type() const;
    QUrl url() const;
};

class Url : public TwitterAPI::Object::Object
{
public:
    Url(const QByteArray &json = NULL);

    QString displayUrl() const;
    QUrl expandedUrl() const;
    QList<int> indices() const;
    QUrl url() const;
};

class UserMention : public TwitterAPI::Object::Object
{
public:
    UserMention(const QByteArray &json = NULL);

    qint64 id() const;
    QString idStr() const;
    QList<int> indices() const;
    QString name() const;
    QString screenName() const;
};

class Entities : public TwitterAPI::Object::Object
{
public:
    Entities(const QByteArray &json = NULL);

    QList<HashTags> hashtags() const;
    QList<Media> media() const;
    QList<Url> urls() const;
    QList<UserMention> userMentions() const;
};
}
}

#endif // ENTITIES_H
