#include "updateprofile.h"
#include "twitter/tweetobject.h"

#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonObject>
#include <QThread>

UpdateProfile::UpdateProfile(QObject *parent) :
    QObject(parent)
{
    try {
        m_myscreenname = m_twitter.verifyCredentials().screen_name();
        //emit stateChanged(GetScreenNameSuccessed);
    } catch(std::runtime_error &e) {
        m_errormessage = QString::fromStdString(e.what());
        //emit stateChanged(GetScreenNameFailed);
    }

    qRegisterMetaType<Update::State>("Update::State");
    //update_name
    connect(&m_updateName, &UpdateName::executed, [&](const UsersObject &user) {
        emit executed(Name, user);
    });
    connect(&m_updateName, &UpdateName::updated, [&](const QString &updatedName) {
        emit updated(Name, updatedName);
    });
    connect(&m_updateName, &UpdateName::resultRecieved, [&]() {
        emit resultRecieved(Name);
    });
    connect(&m_updateName, &UpdateName::error, [&](const Update::State &state, const QString &errorMessage) {
        emit error(Name, state, errorMessage);
    });

    //update_url
    connect(&m_updateUrl, &UpdateUrl::executed, [&](const UsersObject &user) {
        emit executed(Url, user);
    });
    connect(&m_updateUrl, &UpdateUrl::updated, [&](const QString &updatedUrl) {
        emit updated(Url, updatedUrl);
    });
    connect(&m_updateUrl, &UpdateUrl::resultRecieved, [&]() {
        emit resultRecieved(Url);
    });
    connect(&m_updateUrl, &UpdateUrl::error, [&](const Update::State &state, const QString &errorMessage) {
        emit error(Url, state, errorMessage);
    });

    //update_location
    connect(&m_updateLocation, &UpdateLocation::executed, [&](const UsersObject &user) {
        emit executed(Location, user);
    });
    connect(&m_updateLocation, &UpdateLocation::updated, [&](const QString &updatedLocation) {
        emit updated(Location, updatedLocation);
    });
    connect(&m_updateLocation, &UpdateLocation::resultRecieved, [&]() {
        emit resultRecieved(Location);
    });
    connect(&m_updateLocation, &UpdateUrl::error, [&](const Update::State &state, const QString &errorMessage) {
        emit error(Location, state, errorMessage);
    });

    //update_description
    connect(&m_updateDescription, &UpdateDescription::executed, [&](const UsersObject &user) {
        emit executed(Description, user);
    });
    connect(&m_updateDescription, &UpdateDescription::updated, [&](const QString &updatedDescription) {
        emit updated(Description, updatedDescription);
    });
    connect(&m_updateDescription, &UpdateDescription::resultRecieved, [&]() {
        emit resultRecieved(Description);
    });
    connect(&m_updateDescription, &UpdateDescription::error, [&](const Update::State &state, const QString &errorMessage) {
        emit error(Description, state, errorMessage);
    });

    //update_image
    connect(&m_updateImage, &UpdateImage::executed, [&](const UsersObject &user) {
        emit executed(Image, user);
    });
    connect(&m_updateImage, &UpdateImage::updated, [&](const QString &updatedImageUrl) {
        emit updated(Image, updatedImageUrl);
    });
    connect(&m_updateImage, &UpdateImage::resultRecieved, [&]() {
        emit resultRecieved(Image);
    });
    connect(&m_updateImage, &UpdateUrl::error, [&](const Update::State &state, const QString &errorMessage) {
        emit error(Image, state, errorMessage);
    });
}

QString UpdateProfile::updateNameErrorString()
{
    return m_updateName.errorString();
}

QString UpdateProfile::executedUserScreenName()
{
    return m_executeduser;
}

QString UpdateProfile::profileValue()
{
    return m_profilevalue;
}

QString UpdateProfile::errorString()
{
    return m_errormessage;
}

QString UpdateProfile::profileTypeString(const UpdateProfile::ProfileType &type)
{
    switch(type) {
    case Name:
        return tr("name");
    case Url:
        return tr("url");
    case Location:
        return tr("location");
    case Description:
        return tr("description");
    case Image:
        return tr("image");
    }
}

void UpdateProfile::postStartupMessage()
{
    try {
        m_twitter.statusUpdate(m_settings.startupMessage());
    } catch(...) {
        throw;
    }
}

void UpdateProfile::postClosedMessage()
{
    try {
        m_twitter.statusUpdate(m_settings.closedMessage());
    } catch(...) {
        throw;
    }
}

bool UpdateProfile::exec(const QByteArray &twitter_status_object_json_data)
{
    const TweetObject tweet(twitter_status_object_json_data);
    const QString text = tweet.text();
    const QString user_screen_name = tweet.user().screen_name();
    const QRegExp update_name_reg_exp1("^.*@" + m_myscreenname + "\\s+update_name\\s+.*");
    const QRegExp update_name_reg_exp2("^\\s*.+\\s*\\(@" + m_myscreenname + "\\).*$");
    const QRegExp update_url_reg_exp("^.*@" + m_myscreenname + "\\s+update_url\\s+.+");
    const QRegExp update_location_reg_exp("^.*@" + m_myscreenname + "\\s+update_location\\s+.+");
    const QRegExp update_description_reg_exp("^.*@" + m_myscreenname + "\\s+update_description\\s+.+");
    bool executedUpdate = true;

    if(!text.isEmpty() && !text.startsWith("RT")) {
        if(update_name_reg_exp1.exactMatch(text) && m_settings.isEnabledUpdateName()) {
            m_executeduser = user_screen_name;
            m_profilevalue = text;
            m_profilevalue.remove(QRegExp("^.*@" + m_myscreenname + ("\\s+update_name\\s+")));
            m_updateName.exec(tweet, m_profilevalue);
        } else if(update_name_reg_exp2.exactMatch(text) && m_settings.isEnabledUpdateName()) {
            m_executeduser = user_screen_name;
            m_profilevalue = text;
            m_profilevalue.remove(QRegExp("\\s*\\(@" + m_myscreenname + "\\).*$"));
            m_updateName.exec(tweet, m_profilevalue);
        } else if(update_url_reg_exp.exactMatch(text) && m_settings.isEnabledUpdateUrl()) {
            m_executeduser = user_screen_name;
            m_profilevalue = text;
            m_profilevalue.remove(QRegExp("^.*@" + m_myscreenname + ("\\s+update_url\\s+")));
            m_updateUrl.exec(tweet, m_profilevalue);
        } else if(update_location_reg_exp.exactMatch(text) && m_settings.isEnabledUpdateLocation()) {
            m_executeduser = user_screen_name;
            m_profilevalue = text;
            m_profilevalue.remove(QRegExp("^.*@" + m_myscreenname + ("\\s+update_location\\s+")));
            m_updateLocation.exec(tweet, m_profilevalue);
        } else if(update_description_reg_exp.exactMatch(text) && m_settings.isEnabledUpdateDescription()) {
        m_executeduser = user_screen_name;
        m_profilevalue = text;
        m_profilevalue.remove(QRegExp("^.*@" + m_myscreenname + ("\\s+update_description\\s+")));
        m_updateDescription.exec(tweet, m_profilevalue);
        } else {
            executedUpdate = false;
        }
    } else {
        executedUpdate = false;
    }

    emit finished();

    return executedUpdate;
}
