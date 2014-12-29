#ifndef UPDATEPROFILEBANNER_H
#define UPDATEPROFILEBANNER_H

#include "../restapi.h"
#include "../../object/users.h"
#include "../../object/error.h"

#include <QImage>

namespace TwitterAPI {
namespace Rest {
namespace Account {
class UpdateProfileBanner : public RestApi
{
    Q_OBJECT
public:
    UpdateProfileBanner(QObject *parent = 0);
    UpdateProfileBanner(const TwitterAPI::OAuth &oauth, QObject *parent = 0);
signals:
    void finished();
    void successed(const TwitterAPI::Object::Users &user);
    void apiError(const TwitterAPI::Object::Error &error);
    void networkError(const QString &errorMessage);

public slots:
    TwitterAPI::Object::Users exec(const QImage &banner);
    TwitterAPI::Object::Users exec(const QImage &banner, int width, int height, int offsetLeft, int offsetTop);

private:
    static const QUrl ACCOUNT_UPDATE_PROFILE_BANNER_URL;
};
}
}
}

#endif // UPDATEPROFILEBANNER_H
