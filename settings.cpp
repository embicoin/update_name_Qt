#include "settings.h"

const QString Settings::SETTINGS_FILE_NAME                    = "update_name_qt";
const QString Settings::DEFAULT_STARTUP_MESSAGE               = QObject::tr("update_nameが起動されました。");
const QString Settings::DEFAULT_CLOSED_MESSAGE                = QObject::tr("update_nameを終了しました。");
const QString Settings::DEFAULT_UPDATE_NAME_SUCCESSED_MESSAGE = QObject::tr(".@%u nameを\"%n\"に変更しました。");
const QString Settings::DEFAULT_UPDATE_NAME_FAILED_MESSAGE    = QObject::tr(".@%u update_nameに失敗しました。\n%e");

QSettings* Settings::s = new QSettings(QSettings::IniFormat, QSettings::UserScope, Settings::SETTINGS_FILE_NAME);

QString Settings::consumer_key = Settings::s->value("ConsumerKey").toString();
QString Settings::consumer_secret = Settings::s->value("ConsumerSecret").toString();
QString Settings::access_token = Settings::s->value("AccessToken").toString();
QString Settings::access_token_secret = Settings::s->value("AccessTokenSecret").toString();
QString Settings::startup_message = Settings::s->value("StartupMessage", Settings::DEFAULT_STARTUP_MESSAGE).toString();
QString Settings::closed_message = Settings::s->value("ClosedMessage", Settings::DEFAULT_CLOSED_MESSAGE).toString();
QString Settings::update_name_successed_message = Settings::s->value("UpdateNameSuccessedMessage", Settings::DEFAULT_UPDATE_NAME_SUCCESSED_MESSAGE).toString();
QString Settings::update_name_failed_message = Settings::s->value("UpdateNameFailedMessage", Settings::DEFAULT_UPDATE_NAME_FAILED_MESSAGE).toString();
bool Settings::is_post_startup_message = Settings::s->value("IsPostStartupMessage", true).toBool();
bool Settings::is_post_closed_message = Settings::s->value("IsPostClosedMessage", true).toBool();
bool Settings::is_post_udpate_name_successed_message = Settings::s->value("isPostUpdateNameSuccessedMessage", true).toBool();
bool Settings::is_post_update_name_failed_message = Settings::s->value("IsPostUpdateNameFailedMessage", true).toBool();
bool Settings::is_stay_on_system_tray = Settings::s->value("IsStayOnSystemTray", false).toBool();
bool Settings::is_auto_start_update_name = Settings::s->value("IsAutoStartUpdateName", false).toBool();

Settings::Settings()
{
}

Settings::~Settings()
{
}

QString Settings::filePath()
{
    return s->fileName();
}

QString Settings::consumerKey()
{
    return consumer_key;
}

QString Settings::consumerSecret()
{
    return consumer_secret;
}

QString Settings::accessToken()
{
    return access_token;
}

QString Settings::accessTokenSecret()
{
    return access_token_secret;
}

QString Settings::startupMessage()
{
    return startup_message;
}

QString Settings::closedMessage()
{
    return closed_message;
}

QString Settings::updateNameSuccessedMessage()
{
    return update_name_successed_message;
}

QString Settings::updateNameFailedMessage()
{
    return update_name_failed_message;
}

bool Settings::isPostStartupMessage()
{
    return is_post_startup_message;
}

bool Settings::isPostClosedMessage()
{
    return is_post_closed_message;
}

bool Settings::isPostUpdateNameSuccessedMessage()
{
    return is_post_udpate_name_successed_message;
}

bool Settings::isPostUpdateNameFailedMessage()
{
    return is_post_update_name_failed_message;
}

bool Settings::isStayOnSystemTray()
{
    return is_stay_on_system_tray;
}

bool Settings::isAutoStartUpdateName()
{
    return is_auto_start_update_name;
}

void Settings::setConsumerKey(const QString &ck)
{
    consumer_key = ck;
    s->setValue("ConsumerKey", ck);
}

void Settings::setConsumerSecret(const QString &cs)
{
    consumer_secret = cs;
    s->setValue("ConsumerSecret", cs);
}

void Settings::setAccessToken(const QString &at)
{
    access_token = at;
    s->setValue("AccessToken", at);
}

void Settings::setAccessTokenSecret(const QString &ats)
{
    access_token_secret = ats;
    s->setValue("AccessTokenSecret", ats);
}

void Settings::setStartupMessage(const QString &message)
{
    if(message.isEmpty()) {
        startup_message = DEFAULT_STARTUP_MESSAGE;
    } else {
        startup_message = message;
    }
    s->setValue("StartupMessage", startup_message);
}

void Settings::setClosedMessage(const QString &message)
{
    if(message.isEmpty()) {
        closed_message = DEFAULT_CLOSED_MESSAGE;
    } else {
        closed_message = message;
    }
    s->setValue("ClosedMessage", closed_message);
}

void Settings::setUpdateNameSuccessedMessage(const QString &message)
{
    if(message.isEmpty()) {
        update_name_successed_message = DEFAULT_UPDATE_NAME_SUCCESSED_MESSAGE;
    } else {
        update_name_successed_message = message;
    }
    s->setValue("UpdateNameSuccessedMessage", update_name_successed_message);
}

void Settings::setUpdateNameFailedMessage(const QString &message)
{
    if(message.isEmpty()) {
        update_name_failed_message = DEFAULT_UPDATE_NAME_FAILED_MESSAGE;
    } else {
        update_name_failed_message = message;
    }
    s->setValue("UpdateNameFailedMessage", update_name_failed_message);
}

void Settings::setIsPostStartupMessage(const bool on)
{
    is_post_startup_message = on;
    s->setValue("IsPostStartupMessage", on);
}

void Settings::setIsPostClosedMessage(const bool on)
{
    is_post_closed_message = on;
    s->setValue("IsPostClosedMessage", on);
}

void Settings::setIsPostUpdateNameSuccessedMessage(const bool on)
{
    is_post_udpate_name_successed_message = on;
    s->setValue("IsPostUpdateNameSuccessedMessage", on);
}

void Settings::setIsPostUpdateNameFailedMessage(const bool on)
{
    is_post_update_name_failed_message = on;
    s->setValue("IsPostUpdateNameFailedMessage", on);
}

void Settings::setIsStayOnSystemTray(const bool on)
{
    is_stay_on_system_tray = on;
    s->setValue("IsStayOnSystemTray", on);
}

void Settings::setIsAutoStartUpdateName(const bool on)
{
    is_auto_start_update_name = on;
    s->setValue("IsAutoStartUpdateName", on);
}
