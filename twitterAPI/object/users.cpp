#include "users.h"

#include <QJsonDocument>
#include <QJsonValue>
#include <QDebug>

using namespace TwitterAPI::Object;

Users::Users(const QByteArray &json)
    : Object(json)
{
    qDebug() << m_object.isEmpty();
    qRegisterMetaType<TwitterAPI::Object::Users>("TwitterAPI::Object::Users");
}
bool Users::contributorsEnabled() const
{
    return m_object.value("contributors_enabled").toBool();
}

QString Users::createdAt() const
{
    return m_object.value("created_at").toString();
}

bool Users::defaultProfile() const
{
    return m_object.value("default_profile").toBool();
}

bool Users::defaultProfileImage() const
{
    return m_object.value("default_profile_image").toBool();
}

QString Users::description() const
{
    return m_object.value("description").toString();
}

TwitterAPI::Object::Entities Users::entities() const {
    QJsonDocument doc;
    doc.setObject(m_object.value("entities").toObject());
    return doc.toJson();
}

int Users::favouritesCount() const
{
    return m_object.value("favourites_count").toInt();
}

TwitterAPI::triBool Users::followRequestSent() const
{
    QVariant frs = m_object.value("follow_request_sent").toVariant();
    if (frs.isNull())
        return TwitterAPI::Null;
    else if (frs.toBool())
        return TwitterAPI::True;
    else
        return TwitterAPI::False;
}

TwitterAPI::triBool Users::following() const
{
    QVariant f = m_object.value("following").toVariant();
    if (f.isNull())
        return TwitterAPI::Null;
    else if (f.toBool())
        return TwitterAPI::True;
    else
        return TwitterAPI::False;
}

int Users::followersCount() const
{
    return m_object.value("followers_count").toInt();
}

int Users::friendsCount() const
{
    return m_object.value("friends_count").toInt();
}

bool Users::geoEnabled() const
{
    return m_object.value("geo_enabled").toBool();
}

qint64 Users::id() const
{
    return m_object.value("id").toVariant().toLongLong();
}

QString Users::idStr() const
{
    return m_object.value("id_str").toString();
}

bool Users::isTranslator() const
{
    return m_object.value("is_translator").toBool();
}

QLocale Users::lang() const
{
    return QLocale(m_object.value("value").toString());
}

int Users::listedCount() const
{
    return m_object.value("listed_count").toInt();
}

QString Users::location() const
{
    return m_object.value("location").toString();
}

QString Users::name() const
{
    return m_object.value("name").toString();
}

TwitterAPI::triBool Users::notifications() const
{
    QVariant n = m_object.value("notifications").toVariant();

    if (n.isNull())
        return TwitterAPI::Null;
    else if (n.toBool())
        return TwitterAPI::True;
    else
        return TwitterAPI::False;
}

QString Users::profileBackgroundColor() const
{
    return m_object.value("profile_background_color").toString();
}

QUrl Users::profileBackgroundImageUrl() const
{
    return QUrl(m_object.value("profile_background_image_url").toString());
}

QUrl Users::profileBackgroundImageUrlHttps() const
{
    return QUrl(m_object.value("profile_background_image_url_https").toString());
}

bool Users::profileBackgroundTile() const
{
    return m_object.value("profile_background_tile").toBool();
}

QUrl Users::profileBannerUrl() const
{
    return QUrl(m_object.value("profile_banner_url").toString());
}

QUrl Users::profileImageUrl() const
{
    return QUrl(m_object.value("profile_image_url").toString());
}

QUrl Users::profileImageUrlHttps() const
{
    return QUrl(m_object.value("profile_image_url_https").toString());
}

QString Users::profileLinkColor() const
{
    return m_object.value("profile_link_color").toString();
}

QString Users::profileSidebarBorderColor() const
{
    return m_object.value("profile_sidebar_border_color").toString();
}

QString Users::profileSidebarFillColor() const
{
    return m_object.value("profile_sidebar_fill_color").toString();
}

QString Users::profileTextColor() const
{
    return m_object.value("profile_text_color").toString();
}

bool Users::profileUseBackgroundImage() const
{
    return m_object.value("profile_use_background_image").toBool();
}

bool Users::_protected() const
{
    return m_object.value("protected").toBool();
}

QString Users::screenName() const
{
    return m_object.value("screen_name").toString();
}

bool Users::showAllInlineMedia() const
{
    return m_object.value("show_all_inline_media").toBool();
}

Tweets Users::status() const
{
    QJsonDocument doc;
    doc.setObject(m_object.value("status").toObject());
    return Tweets(doc.toJson());
}

int Users::statusesCount() const
{
    return m_object.value("statuses_count").toInt();
}

QString Users::timeZone() const
{
    return m_object.value("time_zone").toString();
}

QUrl Users::url() const
{
    return QUrl(m_object.value("url").toString());
}

int Users::utcOffset() const
{
    return m_object.value("utc_offset").toInt();
}

bool Users::verified() const
{
    return m_object.value("verified").toBool();
}

QString Users::WithheldInCountries() const
{
    return m_object.value("withheld_in_countries").toString();
}

QString Users::withheldScope() const
{
    return m_object.value("withheld_scope").toString();
}

//EntitiesObject
