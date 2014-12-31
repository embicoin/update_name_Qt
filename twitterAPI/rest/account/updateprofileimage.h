#ifndef UPDATEPROFILEIMAGE_H
#define UPDATEPROFILEIMAGE_H

#include "../restapi.h"
#include "../../object/users.h"
#include "../../object/error.h"

namespace TwitterAPI {
namespace Rest {
namespace Account {
class UpdateProfileImage : public RestApi
{
    Q_OBJECT
public:
    UpdateProfileImage(QObject *parent = 0);
    UpdateProfileImage(const TwitterAPI::OAuth &oauth, QObject *parent = 0);
signals:
    void finished();
    void successed(const TwitterAPI::Object::Users &user);
    void apiError(const TwitterAPI::Object::Error &error);
    void networkError(const QString &errorMessage);

public slots:
    TwitterAPI::Object::Users exec(const QByteArray &imageData);

private:
    static const QUrl ACCOUNT_UPDATE_PROFILE_IMAGE_URL;
};
}
}
}

#endif // UPDATEPROFILEIMAGE_H
