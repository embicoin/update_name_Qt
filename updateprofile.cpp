#include "updateprofile.h"
#include "tweetobject.h"

#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonObject>

UpdateProfile::UpdateProfile(QObject *parent) :
    QObject(parent)
{
    try {
        my_screen_name = twitter.getScreenName();
        //emit stateChanged(GetScreenNameSuccessed);
    } catch(std::runtime_error &e) {
        error_message = QString::fromStdString(e.what());
        //emit stateChanged(GetScreenNameFailed);
    }

    qRegisterMetaType<Update::State>("Update::State");
    connect(&update_name, &UpdateName::stateChanged, [&](Update::State state) {
        emit stateChanged(state, Name);
    });
    connect(&update_name, &UpdateName::error, [&](const QString error_string) {
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
    //QJsonObject status_object = QJsonDocument::fromJson(twitter_status_object_json_data).object();
    //QJsonObject user_object = status_object.value("user").toObject();
    //const QString text = status_object.value("text").toString();
    //const QString status_id = status_object.value("id_str").toString();
    //const QString user_screen_name = user_object.value("screen_name").toString();
    const TweetObject tweet(twitter_status_object_json_data);
    const QString text = tweet.text();
    const QString user_screen_name = tweet.user().screen_name();
    const QRegExp update_name_reg_exp1("^.*@" + my_screen_name + "\\s+update_name\\s+.*");
    const QRegExp update_name_reg_exp2("^\\s*.+\\s*\\(@" + my_screen_name + "\\).*$");
    //const QRegExp update_url_reg_exp("^.*@" + my_screen_name + "\\s+update_url\\s+.+");
    //const QRegExp update_location_reg_exp("^.*@" + my_screen_name + "\\s+update_location\\s+.+");
    //const QRegExp update_description_reg_exp("^.*@" + my_screen_name + "\\s+update_description\\s+.+");

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
    /*} else if(update_url_reg_exp.exactMatch(text)) {
        executed_user = user_screen_name;
        operand = text;
        operand.remove(QRegExp("^.*@" + my_screen_name + ("\\s+update_url\\s+")));
        updateUrl(operand);
    } else if(update_location_reg_exp.exactMatch(text)) {
        executed_user = user_screen_name;
        operand = text;
        operand.remove(QRegExp("^.*@" + my_screen_name + ("\\s+update_location\\s+")));
        updateLocation(operand);
    } else if(update_description_reg_exp.exactMatch(text)) {
        executed_user = user_screen_name;
        operand = text;
        operand.remove(QRegExp("^.*@" + my_screen_name + ("\\s+update_description\\s+")));
        updateDescription(operand);*/
    } else {
        return;
    }
/*
    if(new_name.length() > 20) {
        emit stateChanged(Aborted);
        try {
            twitter.statusUpdate(".@" + user_screen_name + tr("名前は20文字までですよ。"), status_id);
            emit stateChanged(ResultRecieved);
        } catch(std::runtime_error &e) {
            error_message = QString::fromStdString(e.what());
            emit stateChanged(RecieveResultFailed);
        }
        return;
    }

    try {
        twitter.updateName(new_name);
    } catch(std::runtime_error &e) {
        error_message = QString::fromStdString(e.what());
        emit stateChanged(UpdateNameFailed);

        if(settings.isPostUpdateNameFailedMessage()) {
            try {
                twitter.statusUpdate(settings.updateNameFailedMessage()
                                     .replace("%u", user_screen_name)
                                     .replace("%n", new_name)
                                     .replace("%e", QString::fromStdString(e.what())), status_id);
                emit stateChanged(ResultRecieved);
            } catch(std::runtime_error &e) {
                error_message = QString::fromStdString(e.what());
                emit stateChanged(RecieveResultFailed);
            }
        }

        return;
    }

    try {
        updated_name = twitter.getName();
    } catch(...) {
        updated_name = new_name;
    }

    emit stateChanged(NameUpdated);

    if(settings.isPostUpdateNameSuccessedMessage()) {
        try {
            twitter.statusUpdate(settings.updateNameSuccessedMessage()
                                 .replace("%u", user_screen_name)
                                 .replace("%n", new_name), status_id);
            emit stateChanged(ResultRecieved);
        } catch(std::runtime_error &e) {
            error_message = QString::fromStdString(e.what());
            emit stateChanged(RecieveResultFailed);
        }
    }
    */
}
