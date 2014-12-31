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
    : TwitterAPI::Rest::RestApi(oauth, parent)
{
    connect(this, SIGNAL(apiError(TwitterAPI::Object::Error)), this, SIGNAL(finished()));
    connect(this, SIGNAL(networkError(QString)), this, SIGNAL(finished()));
    connect(this, SIGNAL(successed(TwitterAPI::Object::Users)), this, SIGNAL(finished()));
}

TwitterAPI::Rest::Account::UpdateProfileBanner::Result UpdateProfileBanner::exec(const QByteArray &bannerData)
{
    try {
        Result result;
        requestApi(QNetworkAccessManager::PostOperation,
                   ACCOUNT_UPDATE_PROFILE_BANNER_URL,
        {{"banner", bannerData.toBase64()}});
        switch (responseCode()) {
        case 200:
        case 201:
        case 202:
            result = SuccesfullyUploaded;
            break;
        case 400:
            result = ImageWasNotProvidedOrTheImageData;
            break;
        case 422:
            result = ImageIsTooLarge;
            break;
        default:
            result = Failed;
            break;
        }
        emit successed(result);
        return result;
    } catch (const std::runtime_error &e) {
        emit networkError(QString::fromStdString(e.what()));
    } catch (const TwitterAPI::Object::Error &e) {
        emit apiError(e);
    }

    return Failed;
}

TwitterAPI::Rest::Account::UpdateProfileBanner::Result UpdateProfileBanner::exec(const QByteArray &bannerData, int width, int height, int offsetLeft, int offsetTop)
{
    try {
        Result result;
        TwitterAPI::Object::Users user = requestApi(QNetworkAccessManager::PostOperation,
                                                    ACCOUNT_UPDATE_PROFILE_BANNER_URL,
                                                    {{"banner", bannerData.toBase64()},
                                                    {"width", width},
                                                    {"height", height},
                                                    {"offset_left", offsetLeft},
                                                    {"offset_top", offsetTop}});
        switch (responseCode()) {
        case 200:
        case 201:
        case 202:
            result = SuccesfullyUploaded;
            break;
        case 400:
            result = ImageWasNotProvidedOrTheImageData;
            break;
        case 422:
            result = ImageIsTooLarge;
            break;
        default:
            result = Failed;
            break;
        }
        emit successed(result);
        return result;
    } catch (const std::runtime_error &e) {
        emit networkError(QString::fromStdString(e.what()));
    } catch (const TwitterAPI::Object::Error &e) {
        emit apiError(e);
    }

    return Failed;
}
