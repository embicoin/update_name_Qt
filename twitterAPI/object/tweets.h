#ifndef TWEETS_H
#define TWEETS_H

#include "../twitterglobal.h"
#include "users.h"
#include "object.h"
#include "entities.h"

namespace TwitterAPI {
namespace Object {
class Users;

class Tweets : public TwitterAPI::Object::Object
{
public:
    Tweets(const QByteArray &json = NULL);

    QString createdAt() const;
    Entities entities() const;
    int favoriteCount() const;
    TwitterAPI::triBool favorited() const;
    QString filterLevel() const;
    QString idStr() const;
    QString inReplyToScreenName() const;
    QString inReplyToStatusIdStr() const;
    QString inReplyToUserIdStr() const;
    QString lang() const;
    TwitterAPI::triBool possiblySensitive() const;
    int retweetCount() const;
    bool retweeted() const;
    QString source() const;
    QString text() const;
    bool truncated() const;
    TwitterAPI::Object::Users user() const;
    bool withheldCopyright() const;
    QStringList withheldInCountries() const;
    QString withheldScope() const;
};
}
}

#endif // TWEETS_H
