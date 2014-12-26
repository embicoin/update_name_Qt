#include "updateprofile.h"

using namespace TwitterAPI::Rest::Account;

UpdateProfileParameters::UpdateProfileParameters()
{
    qRegisterMetaType<TwitterAPI::Rest::Account::UpdateProfileParameters>("TwitterAPI::Rest::Account::UpdateProfileParameters");
}

const QUrl UpdateProfile::ACCOUNT_UPDATE_PROFILE_URL("https://api.twitter.com/1.1/account/update_profile.json");

UpdateProfile::UpdateProfile(QObject *parent)
    : RestApi(parent)
{
    connect(this, SIGNAL(apiError(TwitterAPI::Object::Error)), this, SIGNAL(finished()));
    connect(this, SIGNAL(networkError(QString)), this, SIGNAL(finished()));
    connect(this, SIGNAL(successed(TwitterAPI::Object::Users)), this, SIGNAL(finished()));
}

UpdateProfile::UpdateProfile(const TwitterAPI::OAuth &oauth, QObject *parent)
    : RestApi(oauth, parent)
{
    connect(this, SIGNAL(apiError(TwitterAPI::Object::Error)), this, SIGNAL(finished()));
    connect(this, SIGNAL(networkError(QString)), this, SIGNAL(finished()));
    connect(this, SIGNAL(successed(TwitterAPI::Object::Users)), this, SIGNAL(finished()));
}

TwitterAPI::Object::Users UpdateProfile::exec(const UpdateProfileParameters &parameters)
{
    QVariantMap params;
    if (!parameters.name.isNull())
        params["name"] = parameters.name;
    if (!parameters.url.isNull())
        params["url"] = parameters.url;
    if (!parameters.location.isNull())
        params["location"] = parameters.location;
    if (!parameters.description.isNull())
        params["description"] = parameters.description;
    if (!parameters.profileLinkColor.isNull())
        params["profile_link_collor"] = parameters.profileLinkColor;
    try {
        TwitterAPI::Object::Users user = requestApi(QNetworkAccessManager::PostOperation, ACCOUNT_UPDATE_PROFILE_URL, params);
        emit successed(user);
        return user;
    } catch (const std::runtime_error &e) {
        emit networkError(QString::fromStdString(e.what()));
    } catch (const TwitterAPI::Object::Error &e) {
        emit apiError(e);
    }

    return TwitterAPI::Object::Users(NULL);
}
