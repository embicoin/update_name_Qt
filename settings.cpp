#include "settings.h"

const QString Settings::SETTINGS_FILE_NAME                           = "update_name_qt";
const QString Settings::DEFAULT_STARTUP_MESSAGE                      = QObject::tr("update_nameが起動されました。");
const QString Settings::DEFAULT_CLOSED_MESSAGE                       = QObject::tr("update_nameを終了しました。");
const QString Settings::DEFAULT_UPDATE_NAME_SUCCESSED_MESSAGE        = QObject::tr(".@%u nameを\"%n\"に変更しました。");
const QString Settings::DEFAULT_UPDATE_NAME_FAILED_MESSAGE           = QObject::tr(".@%u update_nameに失敗しました。\n%e");
const QString Settings::DEFAULT_UPDATE_URL_SUCCESSED_MESSAGE         = QObject::tr(".@%u URLを\"%l\"に変更しました。");
const QString Settings::DEFAULT_UPDATE_URL_FAILED_MESSAGE            = QObject::tr(".@%u URLの変更に失敗しました。\n%e");
const QString Settings::DEFAULT_UPDATE_LOCATION_SUCCESSED_MESSAGE    = QObject::tr(".@%u Locationを\"%l\"に変更しました。");
const QString Settings::DEFAULT_UPDATE_LOCATION_FAILED_MESSAGE       = QObject::tr(".@%u Locationの変更に失敗しました。\n%e");
const QString Settings::DEFAULT_UPDATE_DESCRIPTION_SUCCESSED_MESSAGE = QObject::tr(".@%u Descriptionを変更しました。");
const QString Settings::DEFAULT_UPDATE_DESCRIPTION_FAILED_MESSAGE    = QObject::tr(".@%u Descriptionの変更に失敗しました。\n%e");



QSettings* Settings::s = new QSettings(QSettings::IniFormat, QSettings::UserScope, Settings::SETTINGS_FILE_NAME);



QString Settings::consumer_key        = Settings::s->value("ConsumerKey").toString();
QString Settings::consumer_secret     = Settings::s->value("ConsumerSecret").toString();
QString Settings::access_token        = Settings::s->value("AccessToken").toString();
QString Settings::access_token_secret = Settings::s->value("AccessTokenSecret").toString();



QString Settings::startup_message                      = Settings::s->value("StartupMessage",Settings::DEFAULT_STARTUP_MESSAGE).toString();
QString Settings::closed_message                       = Settings::s->value("ClosedMessage", Settings::DEFAULT_CLOSED_MESSAGE).toString();
QString Settings::update_name_successed_message        = Settings::s->value("UpdateNameSuccessedMessage", Settings::DEFAULT_UPDATE_NAME_SUCCESSED_MESSAGE).toString();
QString Settings::update_name_failed_message           = Settings::s->value("UpdateNameFailedMessage", Settings::DEFAULT_UPDATE_NAME_FAILED_MESSAGE).toString();
QString Settings::update_url_successed_message         = Settings::s->value("UpdateUrlSuccessedMessage", Settings::DEFAULT_UPDATE_URL_SUCCESSED_MESSAGE).toString();
QString Settings::update_url_failed_message            = Settings::s->value("UpdateUrlFailedMessage", Settings::DEFAULT_UPDATE_URL_FAILED_MESSAGE).toString();
QString Settings::update_location_successed_message    = Settings::s->value("UpdateLocationSuccessedMessage", Settings::DEFAULT_UPDATE_LOCATION_SUCCESSED_MESSAGE).toString();
QString Settings::update_location_failed_message       = Settings::s->value("UpdateLocationFailedMessage", Settings::DEFAULT_UPDATE_LOCATION_FAILED_MESSAGE).toString();
QString Settings::update_description_successed_message = Settings::s->value("UpdateDescriptionSuccessedMessage", Settings::DEFAULT_UPDATE_DESCRIPTION_SUCCESSED_MESSAGE).toString();
QString Settings::update_description_failed_message    = Settings::s->value("UpdateDescriptionFailedMessage", Settings::DEFAULT_UPDATE_DESCRIPTION_FAILED_MESSAGE).toString();



bool Settings::is_enabled_update_name        = Settings::s->value("IsEnabledUpdateName", true).toBool();
bool Settings::is_enabled_udpate_url         = Settings::s->value("IsEnabledUpdateUrl", false).toBool();
bool Settings::is_enabled_update_location    = Settings::s->value("IsEnabledUpdateLocation", false).toBool();
bool Settings::is_enabled_update_description = Settings::s->value("IsEnabledUpdateDescription", false).toBool();



bool Settings::is_post_startup_message                      = Settings::s->value("IsPostStartupMessage", true).toBool();
bool Settings::is_post_closed_message                       = Settings::s->value("IsPostClosedMessage", true).toBool();
bool Settings::is_post_update_name_successed_message        = Settings::s->value("isPostUpdateNameSuccessedMessage", true).toBool();
bool Settings::is_post_update_name_failed_message           = Settings::s->value("IsPostUpdateNameFailedMessage", true).toBool();
bool Settings::is_post_update_url_successed_message         = Settings::s->value("IsPostUpdateUrlSuccessedMessage", true).toBool();
bool Settings::is_post_update_url_failed_message            = Settings::s->value("IsPostUpdateUrlFailedMessage", true).toBool();
bool Settings::is_post_update_location_successed_message    = Settings::s->value("IsPostUpdateLocationSuccessedMessage", true).toBool();
bool Settings::is_post_update_location_failed_message       = Settings::s->value("IsPostUpdateLocationFailedMessage", true).toBool();
bool Settings::is_post_update_description_successed_message = Settings::s->value("IsPostUpdateDescriptionSuccessedMessage", true).toBool();
bool Settings::is_post_update_description_failed_message    = Settings::s->value("IsUpdateDescriptionFailedMessage", true).toBool();

bool Settings::is_stay_on_system_tray                = Settings::s->value("IsStayOnSystemTray", false).toBool();
bool Settings::is_auto_start_update_name             = Settings::s->value("IsAutoStartUpdateName", false).toBool();

Settings::Settings()
{
}

Settings::~Settings()
{
}

QString Settings::filePath() const
{
    return s->fileName();
}

QString Settings::consumerKey() const
{
    return consumer_key;
}

QString Settings::consumerSecret() const
{
    return consumer_secret;
}

QString Settings::accessToken() const
{
    return access_token;
}

QString Settings::accessTokenSecret() const
{
    return access_token_secret;
}

QString Settings::startupMessage() const
{
    return startup_message;
}

QString Settings::closedMessage() const
{
    return closed_message;
}

QString Settings::updateNameSuccessedMessage() const
{
    return update_name_successed_message;
}

QString Settings::updateNameFailedMessage() const
{
    return update_name_failed_message;
}

QString Settings::updateUrlSuccessedMessage() const
{
    return update_url_successed_message;
}

QString Settings::updateUrlFailedMessage() const
{
    return update_url_failed_message;
}

QString Settings::updateLocationSuccessedMessage() const
{
    return update_location_successed_message;
}

QString Settings::updateLocationFailedMessage() const
{
    return update_location_failed_message;
}

QString Settings::updateDescriptionSuccessedMessage() const
{
    return update_description_successed_message;
}

QString Settings::updateDescriptionFailedMessage() const
{
    return update_description_failed_message;
}

bool Settings::isEnabledUpdateName() const
{
    return is_enabled_update_name;
}

bool Settings::isEnabledUpdateUrl() const
{
    return is_enabled_udpate_url;
}

bool Settings::isEnabledUpdateLocation() const
{
    return is_enabled_update_location;
}

bool Settings::isEnabledUpdateDescription() const
{
    return is_enabled_update_description;
}

bool Settings::isPostStartupMessage() const
{
    return is_post_startup_message;
}

bool Settings::isPostClosedMessage() const
{
    return is_post_closed_message;
}

bool Settings::isPostUpdateNameSuccessedMessage() const
{
    return is_post_update_name_successed_message;
}

bool Settings::isPostUpdateNameFailedMessage() const
{
    return is_post_update_name_failed_message;
}

bool Settings::isPostUpdateUrlSuccessedMessage() const
{
    return is_post_update_url_successed_message;
}

bool Settings::isPostUpdateUrlFailedMessage() const
{
    return is_post_update_url_failed_message;
}

bool Settings::isPostUpdateLocationSuccessedMessage() const
{
    return is_post_update_location_successed_message;
}

bool Settings::isPostUpdateLocationFailedMessage() const
{
    return is_post_update_location_failed_message;
}

bool Settings::isPostUpdateDescriptionSuccessedMessage() const
{
    return is_post_update_description_successed_message;
}

bool Settings::isPostUpdateDescriptionFailedMessage() const
{
    return is_post_update_description_failed_message;
}

bool Settings::isStayOnSystemTray() const
{
    return is_stay_on_system_tray;
}

bool Settings::isAutoStartUpdateName() const
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

void Settings::setUpdateUrlSuccessedMessage(const QString &message)
{
    if(message.isEmpty()) {
        update_url_successed_message = DEFAULT_UPDATE_URL_SUCCESSED_MESSAGE;
    } else {
        update_url_successed_message = message;
    }
    s->setValue("UpdateUrlSuccessedMessage", update_url_successed_message);
}

void Settings::setUpdateUrlFailedMessage(const QString &message)
{
    if(message.isEmpty()) {
        update_url_failed_message = DEFAULT_UPDATE_URL_FAILED_MESSAGE;
    } else {
        update_url_failed_message = message;
    }
    s->setValue("UpdateUrlFailedMessage", update_url_failed_message);
}

void Settings::setUpdateLocationSuccessedMessage(const QString &message)
{
    if(message.isEmpty()) {
        update_location_successed_message = DEFAULT_UPDATE_LOCATION_SUCCESSED_MESSAGE;
    } else {
        update_location_successed_message = message;
    }
    s->setValue("UpdateLocationSuccessedMessage", update_location_successed_message);
}

void Settings::setUpdateLocationFailedMessage(const QString &message)
{
    if(message.isEmpty()) {
        update_location_failed_message = DEFAULT_UPDATE_LOCATION_FAILED_MESSAGE;
    } else {
        update_location_failed_message = message;
    }
    s->setValue("UpdateLocationFailedMessage", update_location_failed_message);
}

void Settings::setUpdateDescriptionSuccessedMessage(const QString &message)
{
    if(message.isEmpty()) {
        update_description_successed_message = DEFAULT_UPDATE_DESCRIPTION_SUCCESSED_MESSAGE;
    } else {
        update_description_successed_message = message;
    }
    s->setValue("UpdateDescriptionSuccessedMessage", update_description_successed_message);
}

void Settings::setUpdateDescriptionFailedMessage(const QString &message)
{
    if(message.isEmpty()) {
        update_description_failed_message = DEFAULT_UPDATE_DESCRIPTION_FAILED_MESSAGE;
    } else {
        update_description_failed_message = message;
    }
    s->setValue("UpdateDescriptionFailedMessage", update_description_failed_message);
}

void Settings::setUpdateNameEnabled(const bool &enable)
{
    is_enabled_update_name = enable;
    s->setValue("IsEnabledUpdateName", enable);
}

void Settings::setUpdateUrlEnabled(const bool &enable)
{
    is_enabled_udpate_url = enable;
    s->setValue("IsEnabledUpdateUrl", enable);
}

void Settings::setUpdateLocationEnabled(const bool &enable)
{
    is_enabled_update_location = enable;
    s->setValue("IsEnabledUpdateLocation", enable);
}

void Settings::setUpdateDescriptionEnabled(const bool &enable)
{
    is_enabled_update_description = enable;
    s->setValue("IsEnabledUpdateDescription", enable);
}

void Settings::setIsPostStartupMessage(const bool &on)
{
    is_post_startup_message = on;
    s->setValue("IsPostStartupMessage", on);
}

void Settings::setIsPostClosedMessage(const bool &on)
{
    is_post_closed_message = on;
    s->setValue("IsPostClosedMessage", on);
}

void Settings::setIsPostUpdateNameSuccessedMessage(const bool &on)
{
    is_post_update_name_successed_message = on;
    s->setValue("IsPostUpdateNameSuccessedMessage", on);
}

void Settings::setIsPostUpdateNameFailedMessage(const bool &on)
{
    is_post_update_name_failed_message = on;
    s->setValue("IsPostUpdateNameFailedMessage", on);
}

void Settings::setIsPostUpdateUrlSuccessedMessage(const bool &on)
{
    is_post_update_url_successed_message = on;
    s->setValue("IsPostUpdateUrlSuccessedMessage", on);
}

void Settings::setIsPostUpdateUrlFailedMessage(const bool &on)
{
    is_post_update_url_failed_message = on;
    s->setValue("IsPostUpdateUrlFailedMessage", on);
}

void Settings::setIsPostUpdateLocationSuccessedMessage(const bool &on)
{
    is_post_update_location_successed_message = on;
    s->setValue("IsPostUpdateLocationSuccessedMessage", on);
}

void Settings::setIsPostUpdateLocationFailedMessage(const bool &on)
{
    is_post_update_location_failed_message = on;
    s->setValue("IsPostUpdateLocationFailedMessage", on);
}

void Settings::setIsPostUpdateDescriptionSuccessedMessage(const bool &on)
{
    is_post_update_description_successed_message = on;
    s->setValue("IsPostUpdateDescriptionSuccessedMessage", on);
}

void Settings::setIsPostUpdateDescriptionFailedMessage(const bool &on)
{
    is_post_update_description_failed_message = on;
    s->setValue("IsPostUpdateDescriptionFailedMessage", on);
}

void Settings::setIsStayOnSystemTray(const bool &on)
{
    is_stay_on_system_tray = on;
    s->setValue("IsStayOnSystemTray", on);
}

void Settings::setIsAutoStartUpdateName(const bool &on)
{
    is_auto_start_update_name = on;
    s->setValue("IsAutoStartUpdateName", on);
}
