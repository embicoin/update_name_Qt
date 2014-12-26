#ifndef UPDATEPROFILE_H
#define UPDATEPROFILE_H

#include "../restapi.h"
#include "../../object/users.h"
#include "../../object/error.h"

namespace TwitterAPI {
namespace Rest {
namespace Account {
struct UpdateProfileParameters {
    explicit UpdateProfileParameters();
    QString name;
    QUrl url;
    QString location;
    QString description;
    QString profileLinkColor;
};

class UpdateProfile : public RestApi
{
public:
    explicit UpdateProfile(QObject *parent = 0);
    //explicit UpdateProfile(const OAuth &oauth, QObject *parent = 0);

signals:
    void finished();
    void successed(const TwitterAPI::Object::Users &user);
    void apiError(const TwitterAPI::Object::Error &error);
    void networkError(const QString &errorMessage);
};
}
}
}

#endif // UPDATEPROFILE_H
