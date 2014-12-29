#include <QBuffer>

#include "updateprofileimage.h"

using namespace TwitterAPI::Rest::Account;

const QUrl UpdateProfileImage::ACCOUNT_UPDATE_PROFILE_IMAGE_URL("https://api.twitter.com/1.1/account/update_profile_image.json");

UpdateProfileImage::UpdateProfileImage(QObject *parent)
    : TwitterAPI::Rest::RestApi(parent)
{
    connect(this, SIGNAL(apiError(TwitterAPI::Object::Error)), this, SIGNAL(finished()));
    connect(this, SIGNAL(networkError(QString)), this, SIGNAL(finished()));
    connect(this, SIGNAL(successed(TwitterAPI::Object::Users)), this, SIGNAL(finished()));
}

UpdateProfileImage::UpdateProfileImage(const TwitterAPI::OAuth &oauth, QObject *parent)
    : TwitterAPI::Rest::RestApi(oauth, parent)
{
    connect(this, SIGNAL(apiError(TwitterAPI::Object::Error)), this, SIGNAL(finished()));
    connect(this, SIGNAL(networkError(QString)), this, SIGNAL(finished()));
    connect(this, SIGNAL(successed(TwitterAPI::Object::Users)), this, SIGNAL(finished()));
}

TwitterAPI::Object::Users UpdateProfileImage::exec(const QImage &media)
{
    QByteArray data;
    QBuffer buf(&data);
    buf.open(QBuffer::WriteOnly);
    media.save(&buf);
    buf.close();
    try {
        TwitterAPI::Object::Users user = requestApi(QNetworkAccessManager::PostOperation,
                                                    ACCOUNT_UPDATE_PROFILE_IMAGE_URL,
                                                    {{"media", data.toBase64()}});
        emit successed(user);
        return user;
    } catch (const std::runtime_error &e) {
        emit networkError(QString::fromStdString(e.what()));
    } catch (const TwitterAPI::Object::Error &e) {
        emit apiError(e);
    }

    return NULL;
}
