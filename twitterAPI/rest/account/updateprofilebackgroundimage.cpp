#include "updateprofilebackgroundimage.h"

#include <QBuffer>

using namespace TwitterAPI::Rest::Account;

UpdateProfileBackgroundImageparameters::UpdateProfileBackgroundImageparameters()
{
    qRegisterMetaType<TwitterAPI::Rest::Account::UpdateProfileBackgroundImageparameters>("TwitterAPI::Rest::Account::UpdateProfileBackgroundImageparameters");
}

const QUrl UpdateProfileBackgroundImage::ACCOUNT_UPDATE_PROFILE_BACKGROUND_IMAGE_URL("https://api.twitter.com/1.1/account/update_profile_background_image.json");

UpdateProfileBackgroundImage::UpdateProfileBackgroundImage(QObject *parent)
    : TwitterAPI::Rest::RestApi(parent)
{
    connect(this, SIGNAL(apiError(TwitterAPI::Object::Error)), this, SIGNAL(finished()));
    connect(this, SIGNAL(networkError(QString)), this, SIGNAL(finished()));
    connect(this, SIGNAL(successed(TwitterAPI::Object::Users)), this, SIGNAL(finished()));
}

UpdateProfileBackgroundImage::UpdateProfileBackgroundImage(const TwitterAPI::OAuth &oauth, QObject *parent)
    : TwitterAPI::Rest::RestApi(oauth, parent)
{
    connect(this, SIGNAL(apiError(TwitterAPI::Object::Error)), this, SIGNAL(finished()));
    connect(this, SIGNAL(networkError(QString)), this, SIGNAL(finished()));
    connect(this, SIGNAL(successed(TwitterAPI::Object::Users)), this, SIGNAL(finished()));
}

TwitterAPI::Object::Users UpdateProfileBackgroundImage::exec(const QImage &image, bool tile, bool use)
{
    UpdateProfileBackgroundImageparameters p;
    p.image = image;
    p.tile = tile;
    p.use = use;
    return exec(p);
}

TwitterAPI::Object::Users UpdateProfileBackgroundImage::exec(const UpdateProfileBackgroundImageparameters &parameters)
{
    QVariantMap params;
    QByteArray data;
    QBuffer buf(&data);
    buf.open(QBuffer::WriteOnly);
    parameters.image.save(buf);
    buf.close();
    params["image"] = data.toBase64();
    params["tile"] = parameters.tile;
    params["use"] = parameters.use;
    try {
        TwitterAPI::Object::Users user = requestApi(QNetworkAccessManager::PostOperation,
                                                    ACCOUNT_UPDATE_PROFILE_BACKGROUND_IMAGE_URL,
                                                    params);
        emit successed(user);
        return user;
    } catch (const std::runtime_error &e) {
        emit networkError(QString::fromStdString(e.what()));
    } catch (const TwitterAPI::Object::Error &e) {
        emit apiError(e);
    }

    return NULL;
}
