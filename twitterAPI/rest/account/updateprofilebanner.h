#ifndef UPDATEPROFILEBANNER_H
#define UPDATEPROFILEBANNER_H

#include "../restapi.h"
#include "../../object/users.h"
#include "../../object/error.h"

namespace TwitterAPI {
namespace Rest {
namespace Account {
class UpdateProfileBanner : public RestApi
{
    Q_OBJECT
public:
    enum Result {
        SuccesfullyUploaded,
        ImageWasNotProvidedOrTheImageData,
        ImageIsTooLarge,
        Failed,
    };

    UpdateProfileBanner(QObject *parent = 0);
    UpdateProfileBanner(const TwitterAPI::OAuth &oauth, QObject *parent = 0);
signals:
    void finished();
    void successed(TwitterAPI::Rest::Account::UpdateProfileBanner::Result result);
    void apiError(const TwitterAPI::Object::Error &error);
    void networkError(const QString &errorMessage);

public slots:
    TwitterAPI::Rest::Account::UpdateProfileBanner::Result exec(const QByteArray &bannerData);
    TwitterAPI::Rest::Account::UpdateProfileBanner::Result exec(const QByteArray &bannerData, int width, int height, int offsetLeft, int offsetTop);

private:
    static const QUrl ACCOUNT_UPDATE_PROFILE_BANNER_URL;
};
}
}
}

#endif // UPDATEPROFILEBANNER_H
