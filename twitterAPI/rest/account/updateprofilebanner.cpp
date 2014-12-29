#include <QBuffer>

#include "updateprofilebanner.h"

using namespace TwitterAPI::Rest::Account;

const QUrl UpdateProfileBanner::ACCOUNT_UPDATE_PROFILE_BANNER_URL("https://api.twitter.com/1.1/account/update_profile_banner.json");

UpdateProfileBanner::UpdateProfileBanner(QObject *parent)
    : TwitterAPI::Rest::RestApi(parent)
{
    connect(this, SIGNAL(apiError(TwitterAPI::Object::Error)), this, SIGNAL(finished()));
    connect(this, SIGNAL(networkError(QString)), this, SIGNAL(finished()));
    connect(this, SIGNAL(successed(TwitterAPI::Object::Users)), this, SIGNAL(finished()));
}

UpdateProfileBanner::UpdateProfileBanner(const TwitterAPI::OAuth &oauth, QObject *parent)
    : TwitterAPI::Rest::RestApi(parent)
{
    connect(this, SIGNAL(apiError(TwitterAPI::Object::Error)), this, SIGNAL(finished()));
    connect(this, SIGNAL(networkError(QString)), this, SIGNAL(finished()));
    connect(this, SIGNAL(successed(TwitterAPI::Object::Users)), this, SIGNAL(finished()));
}

TwitterAPI::Object::Users UpdateProfileBanner::exec(const QImage &banner)
{
    QByteArray data;
    QBuffer buf(&data);
    buf.open(QBuffer::WriteOnly);
    banner.save(&buf);
    buf.close();
    try {
        TwitterAPI::Object::Users user = requestApi(QNetworkAccessManager::PostOperation,
                                                    ACCOUNT_UPDATE_PROFILE_BANNER_URL,
                                                    {{"banner", data.toBase64()}});
        emit successed(user);
        return user;
    } catch (const std::runtime_error &e) {
        emit networkError(QString::fromStdString(e.what()));
    } catch (const TwitterAPI::Object::Error &e) {
        emit apiError(e);
    }

    return NULL;
}

TwitterAPI::Object::Users UpdateProfileBanner::exec(const QImage &banner, int width, int height, int offsetLeft, int offsetTop)
{
    QByteArray data;
    QBuffer buf(&data);
    buf.open(QBuffer::WriteOnly);
    banner.save(&buf);
    buf.close();
    try {
        TwitterAPI::Object::Users user = requestApi(QNetworkAccessManager::PostOperation,
                                                    ACCOUNT_UPDATE_PROFILE_BANNER_URL,
                                                    {{"banner", data.toBase64()},
                                                    {"width", width},
                                                    {"height", height},
                                                    {"offset_left", offsetLeft},
                                                    {"offset_top", offsetTop}});
        emit successed(user);
        return user;
    } catch (const std::runtime_error &e) {
        emit networkError(QString::fromStdString(e.what()));
    } catch (const TwitterAPI::Object::Error &e) {
        emit apiError(e);
    }

    return NULL;
}
