/*
Copyright 2014 owata_programer

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifndef TWEETS_H
#define TWEETS_H

#include "../twitterglobal.h"

#include <QJsonObject>

namespace TwitterAPI {
namespace Object {
class Tweets
{
public:
    Tweets(const QByteArray &json = NULL);
    void setJson(const QByteArray &json);

    QString createdAt() const;
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
    bool withheldCopyright() const;
    QStringList withheldInCountries() const;
    QString withheldScope() const;

private:
    QJsonObject m_object;
};
}
}

#endif // TWEETS_H
