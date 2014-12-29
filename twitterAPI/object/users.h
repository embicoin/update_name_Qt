#ifndef USERS_H
#define USERS_H

#include "../twitterglobal.h"
#include "tweets.h"
#include "object.h"

#include <QLocale>
#include <QUrl>
#include <QJsonObject>

namespace TwitterAPI {
namespace Object {
class Tweets;

class Users : public TwitterAPI::Object::Object
{
public:
    Users(const QByteArray &json = NULL);

    bool contributorsEnabled() const;
    QString createdAt() const;
    bool defaultProfile() const;
    bool defaultProfileImage() const;
    QString description() const;
    //EntitiesObject entities() const;
    int favouritesCount() const;
    TwitterAPI::triBool followRequestSent() const;
    TwitterAPI::triBool following() const;
    int followersCount() const;
    int friendsCount() const;
    bool geoEnabled() const;
    qint64 id() const;
    QString idStr() const;
    bool isTranslator() const;
    QLocale lang() const;
    int listedCount() const;
    QString location() const;
    QString name() const;
    TwitterAPI::triBool notifications() const;
    QString profileBackgroundColor() const;
    QUrl profileBackgroundImageUrl() const;
    QUrl profileBackgroundImageUrlHttps() const;
    bool profileBackgroundTile() const;
    QUrl profileBannerUrl() const;
    QUrl profileImageUrl() const;
    QUrl profileImageUrlHttps() const;
    QString profileLinkColor() const;
    QString profileSidebarBorderColor() const;
    QString profileSidebarFillColor() const;
    QString profileTextColor() const;
    bool profileUseBackgroundImage() const;
    bool _protected() const;
    QString screenName() const;
    bool showAllInlineMedia() const;
    TwitterAPI::Object::Tweets status() const;
    int statusesCount() const;
    QString timeZone() const;
    QUrl url() const;
    int utcOffset() const;
    bool verified() const;
    QString WithheldInCountries() const;
    QString withheldScope() const;
};
}
}

#endif // USERS_H
