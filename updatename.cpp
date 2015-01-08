#include "updatename.h"
#include "updatenameqtglobal.h"
#include "twitterAPI/rest/users/lookup.h"
#include "twitterAPI/rest/account/updateprofile.h"
#include "twitterAPI/rest/statuses/update.h"
#include "twitterAPI/rest/account/updateprofilebackgroundimage.h"
#include "twitterAPI/rest/account/updateprofilebanner.h"
#include "twitterAPI/rest/account/updateprofileimage.h"

#include <QEventLoop>
#include <QTimer>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <memory>
#include <QImage>
#include <QDebug>

using UpdateNameQt::settings;

UpdateProfile::UpdateProfile(const TwitterAPI::OAuth &oauth, QObject *parent)
      : QObject(parent)
{
    qRegisterMetaType<UpdateProfile::UpdateType>("UpdateProfile::UpdateType");
    m_oauth.setConsumerKey(oauth.consumerKey());
    m_oauth.setConsumerSecret(oauth.consumerSecret());
    m_oauth.setAccessToken(oauth.accessToken());
    m_oauth.setAccessTokenSecret(oauth.accessTokenSecret());
}

QString UpdateProfile::errorString() const
{
    return m_errorMessage;
}

void UpdateProfile::update(UpdateProfile::UpdateType type, const TwitterAPI::Object::Tweets &updateTweet, const QString newProfile)
{
    emit updateStarted(type, updateTweet.user());

    switch (type) {
    //account/update_profileで変更する部分
    case Name:
    case Url:
    case Location:
    case Description: {
        TwitterAPI::Rest::Account::UpdateProfile updateProfile(m_oauth);
        TwitterAPI::Rest::Account::UpdateProfileParameters parameters;

        //パラメータのセット
        switch (type) {
        case Name:
            if (!settings->value("EnabledUpdateName").toBool())
                return;
            //フォーマットのセット
            if (settings->value("EnabledUpdateNameFormat").toBool())
                parameters.name = settings->value("UpdateNameFormat").toString().replace("%{name}", newProfile);
            else
                parameters.name = newProfile;
            break;
        case Url:
            if (!settings->value("EnabledUpdateUrl").toBool())
                return;
            //URLのチェック
            if (settings->value("EnabledCheckUrl").toBool()) {
                QNetworkAccessManager man;
                std::unique_ptr<QNetworkReply> rep;
                QTimer timer;
                QEventLoop el;

                connect(&man, SIGNAL(finished(QNetworkReply*)), &el, SLOT(quit()));
                connect(&timer, SIGNAL(timeout()), &el, SLOT(quit()));

                timer.setSingleShot(true);
                timer.start(TIMEOUT_COUNT_MSEC);
                rep.reset(man.get(QNetworkRequest(updateTweet.entities().urls().isEmpty()
                                                      ? newProfile
                                                      : updateTweet.entities().urls().first().expandedUrl())));
                el.exec();

                if (rep->error() != QNetworkReply::NoError || !timer.isActive()) {
                    emit updateError(Url, timer.isActive() ? rep->errorString() : tr("タイムアウトしました。"));

                    if (settings->value("EnabledUpdateUrlFailedMessage").toBool()) {
                        TwitterAPI::Rest::Statuses::UpdateParameters p;
                        p.status = settings->value("UpdateUrlFailedMessage").toString()
                                .replace("%{screen_name}", updateTweet.user().screenName())
                                .replace("%{error}", timer.isActive() ? rep->errorString() : tr("タイムアウトしました。"));
                        p.inReplyToStatusId = updateTweet.idStr();
                        postResult(p);
                    }
                    emit finished();
                    return;
                }
            }
            parameters.url = newProfile;
            break;
        case Location:
            parameters.location = newProfile;
            break;
        case Description:
            parameters.description = newProfile;
            break;
        default:
            break;
        }

        TwitterAPI::Object::Users user = updateProfile.exec(parameters);
        if (updateProfile.errorString().isEmpty()) {
            QString np;
            bool isPostResult = false;
            switch (type) {
            case Name:
                np = user.name();
                isPostResult = settings->value("EnabledUpdateNameSuccessedMessage").toBool();
                break;
            case Url:
                np = user.url().toString();
                isPostResult = settings->value("EnabledUpdateUrlSuccessedMessage").toBool();
                break;
            case Location:
                np = user.location();
                isPostResult = settings->value("EnabledUpdateLocationSuccessedMessage").toBool();
                break;
            case Description:
                np = user.description();
                isPostResult = settings->value("EnabledUpdateDescriptionSuccessedMessage").toBool();
                break;
            default:
                break;
            }

            emit updateFinished(type, np);

            if (isPostResult) {
                TwitterAPI::Rest::Statuses::UpdateParameters p;
                p.inReplyToStatusId = updateTweet.idStr();

                switch (type) {
                case Name:
                    p.status = settings->value("UpdateNameSuccessedMessage").toString();
                    p.status.replace("%{name}", newProfile);
                    break;
                case Url:
                    p.status = settings->value("UpdateUrlSuccessedMessage").toString();
                    p.status.replace("%{url}", newProfile);
                    break;
                case Location:
                    p.status = settings->value("UpdateLocationSuccessedMessage").toString();
                    p.status.replace("%{location}", newProfile);
                    break;
                case Description:
                    p.status = settings->value("UpdateDescriptionSuccessedMessage").toString();
                    p.status.replace("%{description}", newProfile);
                    break;
                default:
                    break;
                }
                p.status.replace("%{screen_name}", updateTweet.user().screenName());
                postResult(p);
            }
        } else {
            bool isPostResult;

            switch (type) {
            case Name:
                isPostResult = settings->value("EnabledUpdateNameFailedMessage").toBool();
                break;
            case Url:
                isPostResult = settings->value("EnabledUpdateUrlFailedMessage").toBool();
                break;
            case Location:
                isPostResult = settings->value("EnabledUpdateLocationFailedMessage").toBool();
                break;
            case Description:
                isPostResult = settings->value("EnabledUpdateDescriptionFailedMessage").toBool();
                break;
            default:
                return;
            }

            emit updateError(type, updateProfile.errorString());

            if (isPostResult) {
                TwitterAPI::Rest::Statuses::UpdateParameters p;
                p.inReplyToStatusId = updateTweet.idStr();
                switch (type) {
                case Name:
                    p.status = settings->value("UpdateNameFailedMessage").toString();
                    p.status.replace("%{name}", newProfile);
                    break;
                case Url:
                    p.status = settings->value("UpdateUrlFailedMessage").toString();
                    p.status.replace("%{url}", newProfile);
                    break;
                case Location:
                    p.status = settings->value("UpdateLocationFailedMessage").toString();
                    p.status.replace("%{location}", newProfile);
                    break;
                case Description:
                    p.status = settings->value("UpdateDescriptionFailedMessage").toString();
                    p.status.replace("%{description}", newProfile);
                    break;
                default:
                    return;
                }
                p.status.replace("%{screen_name}", updateTweet.user().screenName())
                        .replace("%{error}", updateProfile.errorString());
                postResult(p);
            }
        }
    }
        break;
    case Image:
    case Background:
    case Banner:
        //画像のダウンロード
        QNetworkAccessManager man;
        std::unique_ptr<QNetworkReply> rep;
        QTimer timer;
        QEventLoop el;
        QByteArray image;

        //画像のダウンロード
        connect(&man, SIGNAL(finished(QNetworkReply*)), &el, SLOT(quit()));
        connect(&timer, SIGNAL(timeout()), &el, SLOT(quit()));

        timer.setSingleShot(true);
        timer.start(TIMEOUT_COUNT_MSEC);
        rep.reset(man.get(QNetworkRequest(updateTweet.entities().media().isEmpty()
                                          ? newProfile : updateTweet.entities().media().first().mediaUrlHttps())));
        el.exec();

        if (rep->error() != QNetworkReply::NoError || !timer.isActive()) {
            emit updateError(type, timer.isActive() ? rep->errorString() : tr("タイムアウトしました。"));

            bool isPostResult = false;
            switch (type) {
            case Image:
                isPostResult = settings->value("EnabledUpdateImageFailedMessage").toBool();
                break;
            case Background:
                isPostResult = settings->value("EnabledUpdateBackgroundFailedMessage").toBool();
                break;
            case Banner:
                isPostResult = settings->value("EnabledUpdateBackgroundFailedMessage").toBool();
                break;
            default:
                break;
            }
            if (isPostResult) {
                TwitterAPI::Rest::Statuses::UpdateParameters p;
                p.inReplyToStatusId = updateTweet.idStr();
                switch (type) {
                case Image:
                    p.status = settings->value("UpdateImageFailedMessage").toString()
                            .replace("%{image}", updateTweet.entities().media().isEmpty()
                                     ? newProfile : updateTweet.entities().media().first().mediaUrlHttps().toString());
                    break;
                case Background:
                    p.status = settings->value("UpdateBackgroundFailedMessage").toString()
                            .replace("%{background}", updateTweet.entities().media().isEmpty()
                                     ? newProfile : updateTweet.entities().media().first().mediaUrlHttps().toString());
                    break;
                case Banner:
                    p.status = settings->value("UpdateBannerFailedMessage").toString()
                            .replace("%{banner}", updateTweet.entities().media().isEmpty()
                                     ? newProfile : updateTweet.entities().media().first().mediaUrlHttps().toString());
                    break;
                default:
                    break;
                }
                p.status.replace("%{screen_name}", updateTweet.idStr())
                        .replace("%{error}", timer.isActive() ? rep->errorString() : tr("タイムアウトしました。"));
                postResult(p);
            }
        } else {
            TwitterAPI::Rest::Statuses::UpdateParameters p;
            p.inReplyToStatusId = updateTweet.idStr();

            image = rep->readAll();
            switch (type) {
            case Image: {
                TwitterAPI::Rest::Account::UpdateProfileImage updateImage(m_oauth);
                TwitterAPI::Object::Users user = updateImage.exec(image);

                emit updateFinished(Image, user.profileImageUrlHttps().toString());

                if (updateImage.errorString().isEmpty()) {
                    if (settings->value("EnabledUpdateImageSuccessedMessage").toBool()) {
                        p.status = settings->value("UpdateImageSuccessedMessage").toString()
                                .replace("%{screen_name}", updateTweet.user().screenName())
                                .replace("%{image}", user.profileImageUrlHttps().toString());
                        postResult(p);
                    }
                } else {
                    if (settings->value("EnabledUpdateImageFailedMessage").toBool()) {
                        p.status = settings->value("UpdateImageFailedMessage").toString()
                                .replace("%{screen_name}", updateTweet.user().screenName())
                                .replace("%{image}", updateTweet.entities().media().isEmpty()
                                         ? newProfile : updateTweet.entities().media().first().mediaUrlHttps().toString())
                                .replace("%{error}", updateImage.errorString());
                        postResult(p);
                    }
            }
                break;
            }
            case Background: {
                TwitterAPI::Rest::Account::UpdateProfileBackgroundImage updateBackground(m_oauth);
                TwitterAPI::Object::Users user = updateBackground.exec(image, false, true);

                emit updateFinished(Background, user.profileBackgroundImageUrlHttps().toString());

                if (updateBackground.errorString().isEmpty()) {
                    if (settings->value("EnabledUpdateBackgroundSuccessedMessage").toBool()) {
                        p.status = settings->value("UpdateBackgroundSuccessedMessage").toString()
                                .replace("%{screen_name}", updateTweet.user().screenName())
                                .replace("%{background}", user.profileBackgroundImageUrlHttps().toString());
                        postResult(p);
                    }
                } else {
                    if (settings->value("EnabledUpdateBackgroundFailedMessage").toBool()) {
                        p.status = settings->value("UpdateBackgroundFailedMessage").toString()
                                .replace("%{screen_name}", updateTweet.user().screenName())
                                .replace("%{background}", updateTweet.entities().media().isEmpty()
                                         ? newProfile : updateTweet.entities().media().first().mediaUrlHttps().toString())
                                .replace("%{error}", updateBackground.errorString());
                        postResult(p);
                    }
            }
                break;
            }
            case Banner: {
                TwitterAPI::Rest::Account::UpdateProfileBanner updateBanner(m_oauth);
                TwitterAPI::Rest::Account::UpdateProfileBanner::Result result;
                TwitterAPI::Rest::Users::Lookup lookup(m_oauth);
                TwitterAPI::Rest::Users::LookupParameters lookupParameters;
                TwitterAPI::Object::Users user;

                result = updateBanner.exec(image);
                lookupParameters.userId << settings->value("UserId").toString().toLongLong();
                user = lookup.exec(lookupParameters).first();

                if (result == TwitterAPI::Rest::Account::UpdateProfileBanner::SuccesfullyUploaded) {

                    emit updateFinished(Banner, user.profileBannerUrl().toString());

                    if (updateBanner.errorString().isEmpty()) {
                        if (settings->value("EnabledUpdateBannerSuccessedMessage").toBool()) {
                            p.status = settings->value("UpdateBannerSuccessedMessage").toString()
                                    .replace("%{screen_name}", updateTweet.user().screenName())
                                    .replace("%{banner}", user.profileBannerUrl().toString());
                        postResult(p);
                        }
                    } else {
                        if (settings->value("EnabledUpdateBannerFailedMessage").toBool()) {
                            p.status = settings->value("UpdateBannerFailedMessage").toString()
                                    .replace("%{screen_name}", updateTweet.user().screenName())
                                    .replace("%{banner}", updateTweet.entities().media().isEmpty()
                                             ? newProfile : updateTweet.entities().media().first().mediaUrlHttps().toString())
                                .replace("%{error}", updateBanner.errorString());
                            postResult(p);
                        }
                    }
                } else {
                    if (settings->value("EnabledUpdateImageFailedMessage").toBool()) {
                        QString errmsg;
                        TwitterAPI::Rest::Statuses::UpdateParameters p;

                        if (result == TwitterAPI::Rest::Account::UpdateProfileBanner::ImageWasNotProvidedOrTheImageData)
                            errmsg = tr("画像のデータが正しくありません。");
                        else
                            errmsg = tr("画像のサイズが大きすぎます。");
                        p.inReplyToStatusId = updateTweet.idStr();
                        p.status = settings->value("UpdateImageFailedMessage").toString()
                                .replace("%{screem_name}", updateTweet.user().screenName())
                                .replace("%{image}", updateTweet.entities().media().isEmpty()
                                         ? newProfile : updateTweet.entities().media().first().mediaUrlHttps().toString())
                                .replace("%{error}", errmsg);
                        postResult(p);
                    }
                }
                    break;
                }
            default:
                break;
            }
        }
    }

    emit finished();
}

void UpdateProfile::postResult(TwitterAPI::Rest::Statuses::UpdateParameters parameters)
{
    TwitterAPI::Rest::Statuses::Update update(m_oauth);
    QString status = parameters.status;

    for (;;) {
        update.exec(parameters);

        if (update.errorString().isEmpty()) {
            emit resultPosted();
            break;
        } else {
            emit resultPostError(update.errorString());
            if (update.errorString() == "Status is a duplicate")
                parameters.status = status + UpdateNameQt::someString();
            else
                break;
        }
    }
}

UpdateName::UpdateName(QObject *parent)
    : QThread(parent),
      m_oauth(settings->value("ConsumerKey").toString(), settings->value("ConsumerSecret").toString(),
              settings->value("AccessToken").toString(), settings->value("AccessTokenSecret").toString())
{
    m_userStream = new TwitterAPI::Streaming::User(m_oauth);

    connect(m_userStream, SIGNAL(waitting(uint)), this, SIGNAL(waitting(uint)));
    connect(this, SIGNAL(stopping()), m_userStream, SLOT(stop()));
    connect(m_userStream, &TwitterAPI::Streaming::User::error, [&](const QString &errorMessage) {
        emit error(UserStream, errorMessage);
    });
    //connect(this, SIGNAL(finished()), this, SLOT(postQuitMessage()));
}

UpdateName::~UpdateName()
{
    m_userStream->stop();
    m_userStream->wait();
    m_userStream->deleteLater();
}

void UpdateName::run()
{
    QEventLoop loop;
    TwitterAPI::Rest::Users::Lookup lookup(m_oauth);
    TwitterAPI::Rest::Users::LookupParameters p;

    p.userId << settings->value("UserId").toLongLong();

    connect(m_userStream, SIGNAL(running()), &loop, SLOT(quit()));
    connect(m_userStream, SIGNAL(finished()), &loop, SLOT(quit()));

    emit started();
    m_userStream->start();
    loop.exec();

    disconnect(m_userStream, SIGNAL(running()), &loop, SLOT(quit()));

    //スクリーンネームの取得
    const QList<TwitterAPI::Object::Users> users = lookup.exec(p);
    if (lookup.errorString().isEmpty() && !users.isEmpty()) {
        m_screenName = users.first().screenName();
        emit screenNameLookuped(m_screenName);
    } else {
        emit error(ScreenNameLookup, lookup.errorString());
        return;
    }

    emit running();
    if (settings->value("EnabledStartupMessage").toBool())
        postStartupMessage();

    //connect(m_userStream, SIGNAL(recievedTweet(TwitterAPI::Object::Tweets)), &loop, SLOT(quit()));
    connect(m_userStream, SIGNAL(recievedTweet(TwitterAPI::Object::Tweets)), this, SLOT(startUpdateName(TwitterAPI::Object::Tweets)));
    loop.exec();

    if (settings->value("EnabledQuitMessage").toBool())
        postQuitMessage();
}

void UpdateName::stop()
{
    emit stopping();
}

void UpdateName::startUpdateName(const TwitterAPI::Object::Tweets &tweet)
{
    qDebug() << tweet.text();
    for (const QString &command : UpdateNameQt::updateCommands) {
        if (tweet.text().contains(QRegExp("^.*@" + m_screenName + "\\s+" + command + "\\s+.*"))) {
            const QString profileValue = tweet.text().remove(QRegExp("^.*@" + m_screenName + "\\s+" + command + "\\s+"));
            UpdateProfile updateProfile(m_oauth);
            auto *updateProfileThread = new QThread;

            connect(&updateProfile, SIGNAL(updateStarted(UpdateProfile::UpdateType, TwitterAPI::Object::Users)), this, SIGNAL(updateStarted(UpdateProfile::UpdateType, TwitterAPI::Object::Users)));
            connect(&updateProfile, SIGNAL(updateFinished(UpdateProfile::UpdateType, QString)), this, SIGNAL(updateFinished(UpdateProfile::UpdateType,QString)));
            connect(&updateProfile, SIGNAL(resultPosted()), this, SIGNAL(resultPosted()));
            connect(&updateProfile, SIGNAL(updateError(UpdateProfile::UpdateType, QString)), this, SIGNAL(updateError(UpdateProfile::UpdateType,QString)));
            connect(&updateProfile, SIGNAL(resultPostError(QString)), this, SIGNAL(resultPostError(QString)));
            connect(&updateProfile, SIGNAL(finished()), updateProfileThread, SLOT(quit()));
            connect(this, SIGNAL(stopping()), updateProfileThread, SLOT(quit()));
            connect(updateProfileThread, SIGNAL(finished()), updateProfileThread, SLOT(deleteLater()));

            updateProfile.moveToThread(updateProfileThread);
            updateProfileThread->start();

            if (command == "update_name")
                updateProfile.update(UpdateProfile::Name, tweet, profileValue);
            else if (command == "update_url")
                updateProfile.update(UpdateProfile::Url, tweet, profileValue);
            else if (command == "update_location")
                updateProfile.update(UpdateProfile::Location, tweet, profileValue);
            else if (command == "update_description")
                updateProfile.update(UpdateProfile::Description, tweet, profileValue);
            else if (command == "update_image")
                updateProfile.update(UpdateProfile::Image, tweet, profileValue);
            else if (command == "update_background")
                updateProfile.update(UpdateProfile::Background, tweet, profileValue);
            else if (command == "update_banner")
                updateProfile.update(UpdateProfile::Banner, tweet, profileValue);
            //else
                //continue;
            break;
        }
    }
}

void UpdateName::postStartupMessage()
{
    QString status = settings->value("StartupMessage").toString();
    for (;;) {
        TwitterAPI::Rest::Statuses::Update statusUpdate(m_oauth);
        statusUpdate.exec(status);
        if (statusUpdate.errorString().isEmpty()) {
            emit startupMessagePosted();
            break;
        } else {
            emit startupMessagePostError(statusUpdate.errorString());
            if (statusUpdate.errorString() == "Status is a duplicate.")
                status = settings->value("StartupMessage").toString().append(UpdateNameQt::someString());
            else
                break;
        }
    }
}

void UpdateName::postQuitMessage()
{
    QString status = settings->value("QuitMessage").toString();
    for (;;) {
        TwitterAPI::Rest::Statuses::Update statusUpdate(m_oauth);
        statusUpdate.exec(status);
        if (statusUpdate.errorString().isEmpty()) {
            emit quitMessagePosted();
            break;
        } else {
            emit quitMessagePostError(statusUpdate.errorString());
            if (statusUpdate.errorString() == "Status is a duplicate.")
                status = settings->value("QuitMessage").toString().append(UpdateNameQt::someString());
            else
                break;
        }
    }
}
