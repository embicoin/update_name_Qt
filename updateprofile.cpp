#include "updateprofile.h"
#include "twitter/tweetobject.h"

#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonObject>
#include <QThread>
#include <QDebug>

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
    return NULL;
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
    const QRegExp updateNameRegExp("^\\s*.+\\s*\\(@" + m_myscreenname + "\\).*$");
    bool executedUpdate = false;
    QString command;

    if(!tweet.text().isEmpty() && !tweet.text().startsWith("RT")) {
        if(updateNameRegExp.exactMatch(tweet.text()) && m_settings.isEnabledUpdateName()) {
            qDebug() << "update_name Executed";
            m_executeduser = tweet.user().screen_name();
            m_profilevalue = tweet.text();
            m_profilevalue.remove(QRegExp("\\s*\\(@" + m_myscreenname + "\\).*$"));
            m_updateName.exec(tweet, m_profilevalue);
            executedUpdate = true;
        } else {
            foreach(command, updateCommands) {
                const QRegExp updateRegExp("^.*@" + m_myscreenname + "\\s+update_" + command + "\\s+.*");
                if(updateRegExp.exactMatch(tweet.text())) {
                    m_executeduser = tweet.user().screen_name();
                    m_profilevalue = tweet.text();
                    m_profilevalue.remove(QRegExp("^.*@" + m_myscreenname + "\\s+update_" + command + "\\s+"));
                    executedUpdate = true;
                    break;
                }
            }
            if(executedUpdate) {
                if(command == "name" && m_settings.isEnabledUpdateName()) {
                    qDebug() << "update_name Executed";
                    m_updateName.exec(tweet, m_profilevalue);
                } else if(command == "url" && m_settings.isEnabledUpdateUrl()) {
                    qDebug() << "update_url Executed";
                    m_updateUrl.exec(tweet, m_profilevalue);
                } else if(command == "location" && m_settings.isEnabledUpdateLocation()) {
                    qDebug() << "update_location Executed";
                    m_updateLocation.exec(tweet, m_profilevalue);
                } else if(command == "description" && m_settings.isEnabledUpdateDescription()) {
                    qDebug() << "update_description Executed";
                    m_updateDescription.exec(tweet, m_profilevalue);
                } else if(command == "image" && m_settings.isEnabledUpdateImage()) {
                    qDebug() << "update_image Executed";
                    m_updateImage.exec(tweet);
                } else {
                    qWarning() << "Unknown Command: " << command;
                    executedUpdate = false;
                }
            }
        }
    }

    emit finished();

    return executedUpdate;
}
