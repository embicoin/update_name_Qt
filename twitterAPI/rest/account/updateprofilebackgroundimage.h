#ifndef UPDATEPROFILEBACKGROUNDIMAGE_H
#define UPDATEPROFILEBACKGROUNDIMAGE_H

#include "../restapi.h"
#include "../../object/users.h"
#include "../../object/error.h"

namespace TwitterAPI {
namespace Rest {
namespace Account {

struct UpdateProfileBackgroundImageparameters {
    explicit UpdateProfileBackgroundImageparameters();
    QByteArray imageData;
    bool tile = false;
    bool use = false;
};

class UpdateProfileBackgroundImage : public RestApi
{
    Q_OBJECT
public:
    UpdateProfileBackgroundImage(QObject *parent = 0);
    UpdateProfileBackgroundImage(const TwitterAPI::OAuth &oauth, QObject *parent = 0);
signals:
    void finished();
    void successed(const TwitterAPI::Object::Users &user);
    void apiError(const TwitterAPI::Object::Error &error);
    void networkError(const QString &errorMessage);

public slots:
    TwitterAPI::Object::Users exec(const QByteArray &imageData = NULL, bool tile = false, bool use = false);
    TwitterAPI::Object::Users exec(const TwitterAPI::Rest::Account::UpdateProfileBackgroundImageparameters &parameters);

private:
    static const QUrl ACCOUNT_UPDATE_PROFILE_BACKGROUND_IMAGE_URL;
};
}
}
}

#endif // UPDATEPROFILEBACKGROUNDIMAGE_H
