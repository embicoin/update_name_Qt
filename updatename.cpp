#include "updatename.h"
#include "updatenameqtglobal.h"
#include "twitterAPI/rest/users/lookup.h"
#include "twitterAPI/rest/account/updateprofile.h"
#include "twitterAPI/rest/statuses/update.h"

#include <QEventLoop>
#include <QDebug>

using UpdateNameQt::settings;

UpdateName::UpdateName(QObject *parent)
    : QThread(parent)
{
    m_oauth.setConsumerKey(settings->value("ConsumerKey").toString());
    m_oauth.setConsumerSecret(settings->value("ConsumerSecret").toString());
    m_oauth.setAccessToken(settings->value("AccessToken").toString());
    m_oauth.setAccessTokenSecret(settings->value("AccessTokenSecret").toString());

    m_userStream = new TwitterAPI::Streaming::User(m_oauth);
}

UpdateName::~UpdateName()
{
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
        qDebug() << m_screenName;
        emit screenNameLookuped(m_screenName);
    } else {
        qDebug() << lookup.errorString();
        error(ScreenNameLookup, lookup.errorString());
        return;
    }

    connect(m_userStream, SIGNAL(recievedTweet(TwitterAPI::Object::Tweets)), &loop, SLOT(quit()));
    connect(m_userStream, &TwitterAPI::Streaming::User::recievedTweet, [&](const TwitterAPI::Object::Tweets &tweet) {
        for (const QString &command : UpdateNameQt::updateCommands) {
            //qDebug() << command;
            if (tweet.text().contains(QRegExp("\\s*@" + m_screenName + "\\s*" + command + "\\s+\\s*"))) {
                const QString profileValue = tweet.text().remove(QRegExp("\\s*@" + m_screenName + "\\s*" + command + "\\s+"));
                QString profileType = command.mid(7);

                profileType.replace(0, 1, profileType.at(0).toUpper());
                //qDebug() << profileType;

                if (!settings->value("EnabledUpdate" + profileType).toBool())
                    return;
                if (profileType == "Name"
                        || profileType == "Url"
                        || profileType == "Location"
                        || profileType == "Description") {
                    TwitterAPI::Rest::Account::UpdateProfile updateProfile(m_oauth);
                    TwitterAPI::Rest::Account::UpdateProfileParameters updateProfileParameters;
                    auto *updateProfileThread = new QThread;
                    UpdateType type;

                    if (profileType == "Name") {
                        updateProfileParameters.name = profileValue;
                        //qDebug() << profileValue;
                        type = Name;
                    } else if (profileType == "Url") {
                        updateProfileParameters.url = profileValue;
                        type = Url;
                    } else if (profileType == "Location") {
                        updateProfileParameters.location = profileValue;
                        type = Location;
                    } else if (profileType == "Description") {
                        updateProfileParameters.description = profileValue;
                        type = Description;
                    } else {
                        return;
                    }
                    connect(&updateProfile, &TwitterAPI::Rest::Account::UpdateProfile::successed, [&, type, profileType, tweet](const TwitterAPI::Object::Users &user) {
                        QString updatedProfile;
                        switch (type) {
                        case Name:
                            updatedProfile = user.name();
                            break;
                        case Url:
                            updatedProfile = user.url().toString();
                            break;
                        case Location:
                            updatedProfile = user.location();
                            break;
                        case Description:
                            updatedProfile = user.description();
                            break;
                        default:
                            break;
                        }
                        emit updated(type, updatedProfile);

                        if (settings->value("EnabledUpdate" + profileType + "SuccessedMessage").toBool()) {
                            TwitterAPI::Rest::Statuses::UpdateParameters parameters;
                            parameters.status = settings->value("Update" + profileType + "SuccessedMessage").toString()
                                    .replace("%{screen_name}", tweet.user().screenName());
                            parameters.inReplyToStatusId = tweet.idStr();
                            switch (type) {
                            case Name:
                                parameters.status.replace("%{name}", user.name());
                                break;
                            case Url:
                                parameters.status.replace("%{url}", user.url().toString());
                                break;
                            case Location:
                                parameters.status.replace("%{location}", user.location());
                                break;
                            case Description:
                                parameters.status.replace("%{description}", user.description());
                                break;
                            default:
                                break;
                            }
                            QMetaObject::invokeMethod(this, "postResult", Qt::QueuedConnection, Q_ARG(TwitterAPI::Rest::Statuses::UpdateParameters, parameters));
                        }
                    });
                    connect(&updateProfile, &TwitterAPI::Rest::Account::UpdateProfile::apiError, [&, type](const TwitterAPI::Object::Error &error) {
                        emit updateError(type, error.message());
                        //qDebug() << error.message();
                    });
                    connect(&updateProfile, &TwitterAPI::Rest::Account::UpdateProfile::networkError, [&, type](const QString &errorMessage) {
                        emit updateError(type, errorMessage);
                        //qDebug() << errorMessage;
                    });

                    updateProfile.moveToThread(updateProfileThread);
                    updateProfileThread->start();
                    updateProfile.exec(updateProfileParameters);
                }
                break;
            }
        }
    });
    while (m_userStream->isRunning())
        loop.exec();
}

void UpdateName::postResult(const TwitterAPI::Rest::Statuses::UpdateParameters &parameters)
{
    TwitterAPI::Rest::Statuses::Update update(m_oauth);
    auto *updateThread = new QThread;

    qDebug() << "post";

    connect(&update, SIGNAL(successed(TwitterAPI::Object::Tweets)), this, SIGNAL(resultPosted()));
    connect(&update, &TwitterAPI::Rest::Statuses::Update::apiError, [&](const TwitterAPI::Object::Error &error) {
        emit this->error(PostResultTweet, error.message());
        qDebug() << error.message();
    });
    connect(&update, &TwitterAPI::Rest::Statuses::Update::networkError, [&](const QString errorMessage) {
        emit error(PostResultTweet, errorMessage);
    });
    connect(&update, SIGNAL(finished()), updateThread, SLOT(quit()));
    connect(updateThread, SIGNAL(finished()), updateThread, SLOT(deleteLater()));

    update.moveToThread(updateThread);
    updateThread->start();
    update.exec(parameters);
}
