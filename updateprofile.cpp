#include "updateprofile.h"
#include "twitter/tweetobject.h"

#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonObject>

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
    connect(&m_updatename, &UpdateName::stateChanged, [&](Update::State state) {
        emit stateChanged(state, Name);
    });
    connect(&m_updatename, &UpdateName::error, [&](const QString &error_string) {
        m_errormessage = error_string;
    });

    //update_url
    connect(&m_updateurl, &UpdateUrl::stateChanged, [&](Update::State state) {
        emit stateChanged(state, Url);
    });
    connect(&m_updateurl, &UpdateUrl::error, [&](const QString &error_string) {
        m_errormessage = error_string;
    });

    //update_location
    connect(&m_updatelocation, &UpdateLocation::stateChanged, [&](Update::State state) {
        emit stateChanged(state, Location);
    });
    connect(&m_updatelocation, &UpdateLocation::error, [&](const QString &error_string) {
        m_errormessage = error_string;
    });

    //update_description
    connect(&m_udpatedescription, &UpdateDescription::stateChanged, [&](Update::State state) {
        emit stateChanged(state, Description);
    });
    connect(&m_udpatedescription, &UpdateDescription::error, [&](const QString &error_string) {
        m_errormessage = error_string;
    });
}

QString UpdateProfile::updateNameErrorString()
{
    return m_updatename.errorString();
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

void UpdateProfile::exec(const QByteArray &twitter_status_object_json_data)
{
    const TweetObject tweet(twitter_status_object_json_data);
    const QString text = tweet.text();
    const QString user_screen_name = tweet.user().screen_name();
    const QRegExp update_name_reg_exp1("^.*@" + m_myscreenname + "\\s+update_name\\s+.*");
    const QRegExp update_name_reg_exp2("^\\s*.+\\s*\\(@" + m_myscreenname + "\\).*$");
    const QRegExp update_url_reg_exp("^.*@" + m_myscreenname + "\\s+update_url\\s+.+");
    const QRegExp update_location_reg_exp("^.*@" + m_myscreenname + "\\s+update_location\\s+.+");
    const QRegExp update_description_reg_exp("^.*@" + m_myscreenname + "\\s+update_description\\s+.+");

    if(text.isEmpty() || text.startsWith("RT")) {
        return;
    }

    if(update_name_reg_exp1.exactMatch(text) && m_settings.isEnabledUpdateName()) {
        m_executeduser = user_screen_name;
        m_profilevalue = text;
        m_profilevalue.remove(QRegExp("^.*@" + m_myscreenname + ("\\s+update_name\\s+")));
        m_updatename.exec(tweet, m_profilevalue);
    } else if(update_name_reg_exp2.exactMatch(text) && m_settings.isEnabledUpdateName()) {
        m_executeduser = user_screen_name;
        m_profilevalue = text;
        m_profilevalue.remove(QRegExp("\\s*\\(@" + m_myscreenname + "\\).*$"));
        m_updatename.exec(tweet, m_profilevalue);
    } else if(update_url_reg_exp.exactMatch(text) && m_settings.isEnabledUpdateUrl()) {
        m_executeduser = user_screen_name;
        m_profilevalue = text;
        m_profilevalue.remove(QRegExp("^.*@" + m_myscreenname + ("\\s+update_url\\s+")));
        m_updateurl.exec(tweet, m_profilevalue);
    } else if(update_location_reg_exp.exactMatch(text) && m_settings.isEnabledUpdateLocation()) {
        m_executeduser = user_screen_name;
        m_profilevalue = text;
        m_profilevalue.remove(QRegExp("^.*@" + m_myscreenname + ("\\s+update_location\\s+")));
        m_updatelocation.exec(tweet, m_profilevalue);
    } else if(update_description_reg_exp.exactMatch(text) && m_settings.isEnabledUpdateDescription()) {
        m_executeduser = user_screen_name;
        m_profilevalue = text;
        m_profilevalue.remove(QRegExp("^.*@" + m_myscreenname + ("\\s+update_description\\s+")));
        m_udpatedescription.exec(tweet, m_profilevalue);
    }
}
