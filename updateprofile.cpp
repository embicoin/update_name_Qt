#include "updateprofile.h"
#include "twitter/tweetobject.h"

#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonObject>

UpdateProfile::UpdateProfile(QObject *parent) :
    QObject(parent)
{
    try {
        my_screen_name = twitter.verifyCredentials().screen_name();
        //emit stateChanged(GetScreenNameSuccessed);
    } catch(std::runtime_error &e) {
        error_message = QString::fromStdString(e.what());
        //emit stateChanged(GetScreenNameFailed);
    }

    qRegisterMetaType<Update::State>("Update::State");
    //update_name
    connect(&update_name, &UpdateName::stateChanged, [&](Update::State state) {
        emit stateChanged(state, Name);
    });
    connect(&update_name, &UpdateName::error, [&](const QString &error_string) {
        error_message = error_string;
    });

    //update_url
    connect(&update_url, &UpdateUrl::stateChanged, [&](Update::State state) {
        emit stateChanged(state, Url);
    });
    connect(&update_url, &UpdateUrl::error, [&](const QString &error_string) {
        error_message = error_string;
    });

    //update_location
    connect(&update_location, &UpdateLocation::stateChanged, [&](Update::State state) {
        emit stateChanged(state, Location);
    });
    connect(&update_location, &UpdateLocation::error, [&](const QString &error_string) {
        error_message = error_string;
    });

    //update_description
    connect(&update_description, &UpdateDescription::stateChanged, [&](Update::State state) {
        emit stateChanged(state, Description);
    });
    connect(&update_description, &UpdateDescription::error, [&](const QString &error_string) {
        error_message = error_string;
    });
}

QString UpdateProfile::updateNameErrorString()
{
    return update_name.errorString();
}

QString UpdateProfile::executedUserScreenName()
{
    return executed_user;
}

QString UpdateProfile::profileValue()
{
    return profile_value;
}

QString UpdateProfile::errorString()
{
    return error_message;
}

void UpdateProfile::postStartupMessage()
{
    try {
        twitter.statusUpdate(settings.startupMessage());
    } catch(...) {
        throw;
    }
}

void UpdateProfile::postClosedMessage()
{
    try {
        twitter.statusUpdate(settings.closedMessage());
    } catch(...) {
        throw;
    }
}

void UpdateProfile::exec(const QByteArray &twitter_status_object_json_data)
{
    const TweetObject tweet(twitter_status_object_json_data);
    const QString text = tweet.text();
    const QString user_screen_name = tweet.user().screen_name();
    const QRegExp update_name_reg_exp1("^.*@" + my_screen_name + "\\s+update_name\\s+.*");
    const QRegExp update_name_reg_exp2("^\\s*.+\\s*\\(@" + my_screen_name + "\\).*$");
    const QRegExp update_url_reg_exp("^.*@" + my_screen_name + "\\s+update_url\\s+.+");
    const QRegExp update_location_reg_exp("^.*@" + my_screen_name + "\\s+update_location\\s+.+");
    const QRegExp update_description_reg_exp("^.*@" + my_screen_name + "\\s+update_description\\s+.+");

    if(text.isEmpty() || text.startsWith("RT")) {
        return;
    }

    if(update_name_reg_exp1.exactMatch(text)) {
        executed_user = user_screen_name;
        profile_value = text;
        profile_value.remove(QRegExp("^.*@" + my_screen_name + ("\\s+update_name\\s+")));
        update_name.exec(tweet, profile_value);
    } else if(update_name_reg_exp2.exactMatch(text)) {
        executed_user = user_screen_name;
        profile_value = text;
        profile_value.remove(QRegExp("\\s*\\(@" + my_screen_name + "\\).*$"));
        update_name.exec(tweet, profile_value);
    } else if(update_url_reg_exp.exactMatch(text)) {
        executed_user = user_screen_name;
        profile_value = text;
        profile_value.remove(QRegExp("^.*@" + my_screen_name + ("\\s+update_url\\s+")));
        update_url.exec(tweet, profile_value);
    } else if(update_location_reg_exp.exactMatch(text)) {
        executed_user = user_screen_name;
        profile_value = text;
        profile_value.remove(QRegExp("^.*@" + my_screen_name + ("\\s+update_location\\s+")));
        update_location.exec(tweet, profile_value);
    } else if(update_description_reg_exp.exactMatch(text)) {
        executed_user = user_screen_name;
        profile_value = text;
        profile_value.remove(QRegExp("^.*@" + my_screen_name + ("\\s+update_description\\s+")));
        update_description.exec(tweet, profile_value);
    }
}
