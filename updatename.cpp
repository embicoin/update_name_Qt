#include "updatename.h"
#include "updatenameqtglobal.h"
#include "twitterAPI/rest/users/lookup.h"
#include "twitterAPI/rest/account/updateprofile.h"
#include "twitterAPI/rest/statuses/update.h"

#include <QEventLoop>
#include <QTimer>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <memory>
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
                rep.reset(man.get(QNetworkRequest(updateTweet.entities().urls().first().expandedUrl())));
                el.exec();

                if (rep->error() != QNetworkReply::NoError || !timer.isActive()) {
                    if (settings->value("EnabledUpdateUrlFailedMessage").toBool()) {
                        TwitterAPI::Rest::Statuses::UpdateParameters p;
                        p.status = settings->value("UpdateUrlFailedMessage").toString()
                                .replace("%{screen_name}", updateTweet.user().screenName())
                                .replace("%{error}", timer.isActive() ? rep->errorString() : tr("タイムアウトしました。"));
                        p.inReplyToStatusId = updateTweet.idStr();
                        postResult(p);
                    }
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
    }

    emit finished();
}

void UpdateProfile::postResult(const TwitterAPI::Rest::Statuses::UpdateParameters &parameters)
{
    TwitterAPI::Rest::Statuses::Update update(m_oauth);

    update.exec(parameters);

    if (update.errorString().isEmpty())
        emit resultPosted();
    else
        emit resultPostError(update.errorString());
}

UpdateName::UpdateName(QObject *parent)
    : QThread(parent),
      m_oauth(settings->value("ConsumerKey").toString(), settings->value("ConsumerSecret").toString(),
              settings->value("AccessToken").toString(), settings->value("AccessTokenSecret").toString())
{
    m_userStream = new TwitterAPI::Streaming::User(m_oauth);
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
    connect(this, SIGNAL(stopping()), m_userStream, SLOT(stop()));
    connect(m_userStream, &TwitterAPI::Streaming::User::error, [&](const QString &errorMessage) {
        emit error(UserStream, errorMessage);
    });

    emit started();
    m_userStream->start();
    loop.exec();

    //スクリーンネームの取得
    const QList<TwitterAPI::Object::Users> users = lookup.exec(p);
    if (lookup.errorString().isEmpty() && !users.isEmpty()) {
        m_screenName = users.first().screenName();
        emit screenNameLookuped(m_screenName);
    } else {
        emit error(ScreenNameLookup, lookup.errorString());
        return;
    }

    connect(m_userStream, SIGNAL(recievedTweet(TwitterAPI::Object::Tweets)), &loop, SLOT(quit()));
    connect(m_userStream, &TwitterAPI::Streaming::User::recievedTweet, [&](const TwitterAPI::Object::Tweets &tweet) {
        for (const QString &command : UpdateNameQt::updateCommands) {
            if (tweet.text().contains(QRegExp("^.*@" + m_screenName + "\\s+" + command + "\\s+.*"))) {
                const QString profileValue = tweet.text().remove(QRegExp("^.*@" + m_screenName + "\\s+" + command + "\\s+"));
                UpdateProfile updateProfile(m_oauth);
                auto *updateProfileThread = new QThread;

                connect(&updateProfile, SIGNAL(finished()), updateProfileThread, SLOT(quit()));
                connect(this, SIGNAL(stopping()), &updateProfile, SLOT(quit()));
                connect(&updateProfile, SIGNAL(finished()), updateProfileThread, SLOT(deleteLater()));

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
                break;
            }
        }
    });
    while (m_userStream->isRunning())
        loop.exec();
}

void UpdateName::stop()
{
    emit stopping();
}
