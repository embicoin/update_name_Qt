#include "settings.h"

const QString Settings::SETTINGS_FILE_NAME = "update_name_qt";
const QString Settings::DEFAULT_STARTUP_MESSAGE = QObject::tr("update_nameが起動されました。");
const QString Settings::DEFAULT_CLOSED_MESSAGE = QObject::tr("update_nameを終了しました。");
const QString Settings::DEFAULT_UPDATE_NAME_SUCCESSED_MESSAGE = QObject::tr(".@%u Nameを\"%n\"に変更しました。");
const QString Settings::DEFAULT_UPDATE_NAME_FAILED_MESSAGE = QObject::tr(".@%u update_nameに失敗しました。\n%e");
const QString Settings::DEFAULT_UPDATE_URL_SUCCESSED_MESSAGE = QObject::tr(".@%u URLを\"%l\"に変更しました。");
const QString Settings::DEFAULT_UPDATE_URL_FAILED_MESSAGE = QObject::tr(".@%u URLの変更に失敗しました。\n%e");
const QString Settings::DEFAULT_UPDATE_LOCATION_SUCCESSED_MESSAGE = QObject::tr(".@%u Locationを\"%l\"に変更しました。");
const QString Settings::DEFAULT_UPDATE_LOCATION_FAILED_MESSAGE = QObject::tr(".@%u Locationの変更に失敗しました。\n%e");
const QString Settings::DEFAULT_UPDATE_DESCRIPTION_SUCCESSED_MESSAGE = QObject::tr(".@%u Descriptionを\"%d\"に変更しました。");
const QString Settings::DEFAULT_UPDATE_DESCRIPTION_FAILED_MESSAGE = QObject::tr(".@%u Descriptionの変更に失敗しました。\n%e");
const QString Settings::DEFAULT_UPDATE_IMAGE_SUCCESSED_MESSAGE = QObject::tr(".@%u プロフィール画像を\"%i\"に変更しました。");
const QString Settings::DEFAULT_UPDATE_IMAGE_FAILED_MESSAGE = QObject::tr(".@%u プロフィール画像の変更に失敗しました。\n%e");

QSettings* Settings::s = new QSettings(QSettings::IniFormat, QSettings::UserScope, Settings::SETTINGS_FILE_NAME);

QString Settings::m_consumerKey = Settings::s->value("ConsumerKey").toString();
QString Settings::m_consumerSecret = Settings::s->value("ConsumerSecret").toString();
QString Settings::m_accessToken = Settings::s->value("AccessToken").toString();
QString Settings::m_accessTokenSecret = Settings::s->value("AccessTokenSecret").toString();
QString Settings::m_userId = Settings::s->value("UserId").toString();
QString Settings::m_screenName = Settings::s->value("ScreenName").toString();

QString Settings::m_startupMessage = Settings::s->value("StartupMessage",Settings::DEFAULT_STARTUP_MESSAGE).toString();
QString Settings::m_closedMessage = Settings::s->value("ClosedMessage", Settings::DEFAULT_CLOSED_MESSAGE).toString();
QString Settings::m_updateNameSuccessedMessage = Settings::s->value("UpdateNameSuccessedMessage",
                                                                    Settings::DEFAULT_UPDATE_NAME_SUCCESSED_MESSAGE).toString();
QString Settings::m_updateNameFailedMessage = Settings::s->value("UpdateNameFailedMessage",
                                                                 Settings::DEFAULT_UPDATE_NAME_FAILED_MESSAGE).toString();
QString Settings::m_updateUrlSuccessedMessage = Settings::s->value("UpdateUrlSuccessedMessage",
                                                                   Settings::DEFAULT_UPDATE_URL_SUCCESSED_MESSAGE).toString();
QString Settings::m_updateUrlFailedMessage = Settings::s->value("UpdateUrlFailedMessage",
                                                                Settings::DEFAULT_UPDATE_URL_FAILED_MESSAGE).toString();
QString Settings::m_updateLocationSuccessedMessage = Settings::s->value("UpdateLocationSuccessedMessage",
                                                                        Settings::DEFAULT_UPDATE_LOCATION_SUCCESSED_MESSAGE).toString();
QString Settings::m_updateLocationFailedMessage = Settings::s->value("UpdateLocationFailedMessage",
                                                                     Settings::DEFAULT_UPDATE_LOCATION_FAILED_MESSAGE).toString();
QString Settings::m_updateDescriptionSuccessedMessage = Settings::s->value("UpdateDescriptionSuccessedMessage",
                                                                           Settings::DEFAULT_UPDATE_DESCRIPTION_SUCCESSED_MESSAGE).toString();
QString Settings::m_updateDescriptionFailedMessage = Settings::s->value("UpdateDescriptionFailedMessage",
                                                                        Settings::DEFAULT_UPDATE_DESCRIPTION_FAILED_MESSAGE).toString();
QString Settings::m_updateImageSuccessedMessage = Settings::s->value("UpdateImageSuccessedMessage",
                                                                     Settings::DEFAULT_UPDATE_IMAGE_SUCCESSED_MESSAGE).toString();
QString Settings::m_updateImageFailedMessage = Settings::s->value("UpdateImageFailedMessage",
                                                                  Settings::DEFAULT_UPDATE_IMAGE_FAILED_MESSAGE).toString();

QString Settings::m_updateNameFormat = Settings::s->value("UpdateNameFormat").toString();

bool Settings::m_isEnabledUpdateName = Settings::s->value("IsEnabledUpdateName", true).toBool();
bool Settings::m_isEnabledUdpateUrl = Settings::s->value("IsEnabledUpdateUrl", false).toBool();
bool Settings::m_isEnabledUpdateLocation = Settings::s->value("IsEnabledUpdateLocation", false).toBool();
bool Settings::m_isEnabledUpdateDescription = Settings::s->value("IsEnabledUpdateDescription", false).toBool();
bool Settings::m_isEnabledUpdateImage = Settings::s->value("IsEnabledUpdateImage", false).toBool();

bool Settings::m_isPostStartupMessage = Settings::s->value("IsPostStartupMessage", true).toBool();
bool Settings::m_isPostClosedMessage = Settings::s->value("IsPostClosedMessage", true).toBool();
bool Settings::m_isPostUpdateNameSuccessedMessage = Settings::s->value("isPostUpdateNameSuccessedMessage", true).toBool();
bool Settings::m_isPostUpdateNameFailedMessage = Settings::s->value("IsPostUpdateNameFailedMessage", true).toBool();
bool Settings::m_isPostUpdateUrlSuccessedMessage = Settings::s->value("IsPostUpdateUrlSuccessedMessage", true).toBool();
bool Settings::m_isPostUpdateUrlFailedMessage = Settings::s->value("IsPostUpdateUrlFailedMessage", true).toBool();
bool Settings::m_isPostUpdateLocationSuccessedMessage = Settings::s->value("IsPostUpdateLocationSuccessedMessage", true).toBool();
bool Settings::m_isPostUpdateLocationFailedMessage = Settings::s->value("IsPostUpdateLocationFailedMessage", true).toBool();
bool Settings::m_isPostUpdateDescriptionSuccessedMessage = Settings::s->value("IsPostUpdateDescriptionSuccessedMessage", true).toBool();
bool Settings::m_isPostUpdateDescriptionFailedMessage = Settings::s->value("IsUpdateDescriptionFailedMessage", true).toBool();
bool Settings::m_isPostUpdateImageSuccessedMessage = Settings::s->value("IsUpdateImageSuccessedMessage", true).toBool();
bool Settings::m_isPostUpdateImageFailedMessage = Settings::s->value("IsPostUpdateFailedMessage", true).toBool();

bool Settings::m_isStayOnSystemTray = Settings::s->value("IsStayOnSystemTray", false).toBool();
bool Settings::m_isAutoStartUpdateName = Settings::s->value("IsAutoStartUpdateName", false).toBool();

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
    return m_consumerKey;
}

QString Settings::consumerSecret() const
{
    return m_consumerSecret;
}

QString Settings::accessToken() const
{
    return m_accessToken;
}

QString Settings::accessTokenSecret() const
{
    return m_accessTokenSecret;
}

QString Settings::userId() const
{
    return m_userId;
}

QString Settings::screenName() const
{
    return m_screenName;
}

QString Settings::startupMessage() const
{
    return m_startupMessage;
}

QString Settings::closedMessage() const
{
    return m_closedMessage;
}

QString Settings::updateNameSuccessedMessage() const
{
    return m_updateNameSuccessedMessage;
}

QString Settings::updateNameFailedMessage() const
{
    return m_updateNameFailedMessage;
}

QString Settings::updateUrlSuccessedMessage() const
{
    return m_updateUrlSuccessedMessage;
}

QString Settings::updateUrlFailedMessage() const
{
    return m_updateUrlFailedMessage;
}

QString Settings::updateLocationSuccessedMessage() const
{
    return m_updateLocationSuccessedMessage;
}

QString Settings::updateLocationFailedMessage() const
{
    return m_updateLocationFailedMessage;
}

QString Settings::updateDescriptionSuccessedMessage() const
{
    return m_updateDescriptionSuccessedMessage;
}

QString Settings::updateDescriptionFailedMessage() const
{
    return m_updateDescriptionFailedMessage;
}

QString Settings::updateImageSuccessedMessage() const
{
    return m_updateImageSuccessedMessage;
}

QString Settings::updateImageFailedMessage() const
{
    return m_updateImageFailedMessage;
}

QString Settings::updateNameFormat() const
{
    return m_updateNameFormat;
}

bool Settings::isEnabledUpdateName() const
{
    return m_isEnabledUpdateName;
}

bool Settings::isEnabledUpdateUrl() const
{
    return m_isEnabledUdpateUrl;
}

bool Settings::isEnabledUpdateLocation() const
{
    return m_isEnabledUpdateLocation;
}

bool Settings::isEnabledUpdateDescription() const
{
    return m_isEnabledUpdateDescription;
}

bool Settings::isEnabledUpdateImage() const
{
    return m_isEnabledUpdateImage;
}

bool Settings::isPostStartupMessage() const
{
    return m_isPostStartupMessage;
}

bool Settings::isPostClosedMessage() const
{
    return m_isPostClosedMessage;
}

bool Settings::isPostUpdateNameSuccessedMessage() const
{
    return m_isPostUpdateNameSuccessedMessage;
}

bool Settings::isPostUpdateNameFailedMessage() const
{
    return m_isPostUpdateNameFailedMessage;
}

bool Settings::isPostUpdateUrlSuccessedMessage() const
{
    return m_isPostUpdateUrlSuccessedMessage;
}

bool Settings::isPostUpdateUrlFailedMessage() const
{
    return m_isPostUpdateUrlFailedMessage;
}

bool Settings::isPostUpdateLocationSuccessedMessage() const
{
    return m_isPostUpdateLocationSuccessedMessage;
}

bool Settings::isPostUpdateLocationFailedMessage() const
{
    return m_isPostUpdateLocationFailedMessage;
}

bool Settings::isPostUpdateDescriptionSuccessedMessage() const
{
    return m_isPostUpdateDescriptionSuccessedMessage;
}

bool Settings::isPostUpdateDescriptionFailedMessage() const
{
    return m_isPostUpdateDescriptionFailedMessage;
}

bool Settings::isPostUpdateImageSuccessedMessage() const
{
    return m_isPostUpdateImageSuccessedMessage;
}

bool Settings::isPostUpdateImageFailedMessage() const
{
    return m_isPostUpdateImageFailedMessage;
}

bool Settings::isStayOnSystemTray() const
{
    return m_isStayOnSystemTray;
}

bool Settings::isAutoStartUpdateName() const
{
    return m_isAutoStartUpdateName;
}

void Settings::setConsumerKey(const QString &ck)
{
    m_consumerKey = ck;
    s->setValue("ConsumerKey", ck);
}

void Settings::setConsumerSecret(const QString &cs)
{
    m_consumerSecret = cs;
    s->setValue("ConsumerSecret", cs);
}

void Settings::setAccessToken(const QString &at)
{
    m_accessToken = at;
    s->setValue("AccessToken", at);
}

void Settings::setAccessTokenSecret(const QString &ats)
{
    m_accessTokenSecret = ats;
    s->setValue("AccessTokenSecret", ats);
}

void Settings::setUserId(const QString &userId)
{
    m_userId = userId;
    s->setValue("UserId", userId);
}

void Settings::setScreenName(const QString &screenName)
{
    m_screenName = screenName;
    s->setValue("ScreenName", screenName);
}

void Settings::setStartupMessage(const QString &message)
{
    if(message.isEmpty()) {
        m_startupMessage = DEFAULT_STARTUP_MESSAGE;
    } else {
        m_startupMessage = message;
    }
    s->setValue("StartupMessage", m_startupMessage);
}

void Settings::setClosedMessage(const QString &message)
{
    if(message.isEmpty()) {
        m_closedMessage = DEFAULT_CLOSED_MESSAGE;
    } else {
        m_closedMessage = message;
    }
    s->setValue("ClosedMessage", m_closedMessage);
}

void Settings::setUpdateNameSuccessedMessage(const QString &message)
{
    if(message.isEmpty()) {
        m_updateNameSuccessedMessage = DEFAULT_UPDATE_NAME_SUCCESSED_MESSAGE;
    } else {
        m_updateNameSuccessedMessage = message;
    }
    s->setValue("UpdateNameSuccessedMessage", m_updateNameSuccessedMessage);
}

void Settings::setUpdateNameFailedMessage(const QString &message)
{
    if(message.isEmpty()) {
        m_updateNameFailedMessage = DEFAULT_UPDATE_NAME_FAILED_MESSAGE;
    } else {
        m_updateNameFailedMessage = message;
    }
    s->setValue("UpdateNameFailedMessage", m_updateNameFailedMessage);
}

void Settings::setUpdateUrlSuccessedMessage(const QString &message)
{
    if(message.isEmpty()) {
        m_updateUrlSuccessedMessage = DEFAULT_UPDATE_URL_SUCCESSED_MESSAGE;
    } else {
        m_updateUrlSuccessedMessage = message;
    }
    s->setValue("UpdateUrlSuccessedMessage", m_updateUrlSuccessedMessage);
}

void Settings::setUpdateUrlFailedMessage(const QString &message)
{
    if(message.isEmpty()) {
        m_updateUrlFailedMessage = DEFAULT_UPDATE_URL_FAILED_MESSAGE;
    } else {
        m_updateUrlFailedMessage = message;
    }
    s->setValue("UpdateUrlFailedMessage", m_updateUrlFailedMessage);
}

void Settings::setUpdateLocationSuccessedMessage(const QString &message)
{
    if(message.isEmpty()) {
        m_updateLocationSuccessedMessage = DEFAULT_UPDATE_LOCATION_SUCCESSED_MESSAGE;
    } else {
        m_updateLocationSuccessedMessage = message;
    }
    s->setValue("UpdateLocationSuccessedMessage", m_updateLocationSuccessedMessage);
}

void Settings::setUpdateLocationFailedMessage(const QString &message)
{
    if(message.isEmpty()) {
        m_updateLocationFailedMessage = DEFAULT_UPDATE_LOCATION_FAILED_MESSAGE;
    } else {
        m_updateLocationFailedMessage = message;
    }
    s->setValue("UpdateLocationFailedMessage", m_updateLocationFailedMessage);
}

void Settings::setUpdateDescriptionSuccessedMessage(const QString &message)
{
    if(message.isEmpty()) {
        m_updateDescriptionSuccessedMessage = DEFAULT_UPDATE_DESCRIPTION_SUCCESSED_MESSAGE;
    } else {
        m_updateDescriptionSuccessedMessage = message;
    }
    s->setValue("UpdateDescriptionSuccessedMessage", m_updateDescriptionSuccessedMessage);
}

void Settings::setUpdateDescriptionFailedMessage(const QString &message)
{
    if(message.isEmpty()) {
        m_updateDescriptionFailedMessage = DEFAULT_UPDATE_DESCRIPTION_FAILED_MESSAGE;
    } else {
        m_updateDescriptionFailedMessage = message;
    }
    s->setValue("UpdateDescriptionFailedMessage", m_updateDescriptionFailedMessage);
}

void Settings::setUpdateNameFormat(const QString &format)
{
    m_updateNameFormat = format;
    s->setValue("UpdateNameFormat", format);
}

void Settings::setUpdateImageSuccessedMessage(const QString &message)
{
    s->setValue("UpdateImageSuccessedMessage", message.isEmpty() ? DEFAULT_UPDATE_IMAGE_SUCCESSED_MESSAGE : message);
}

void Settings::setUpdateImageFailedMessage(const QString &message)
{
    s->setValue("UpdateImageFailedMessage", message.isEmpty() ? DEFAULT_UPDATE_IMAGE_FAILED_MESSAGE : message);
}

void Settings::setUpdateNameEnabled(const bool &enable)
{
    m_isEnabledUpdateName = enable;
    s->setValue("IsEnabledUpdateName", enable);
}

void Settings::setUpdateUrlEnabled(const bool &enable)
{
    m_isEnabledUdpateUrl = enable;
    s->setValue("IsEnabledUpdateUrl", enable);
}

void Settings::setUpdateLocationEnabled(const bool &enable)
{
    m_isEnabledUpdateLocation = enable;
    s->setValue("IsEnabledUpdateLocation", enable);
}

void Settings::setUpdateDescriptionEnabled(const bool &enable)
{
    m_isEnabledUpdateDescription = enable;
    s->setValue("IsEnabledUpdateDescription", enable);
}

void Settings::setUpdateImageEnabled(const bool &enable)
{
    m_isEnabledUpdateImage = enable;
    s->setValue("IsEnabledUpdateImage", enable);
}

void Settings::setIsPostStartupMessage(const bool &on)
{
    m_isPostStartupMessage = on;
    s->setValue("IsPostStartupMessage", on);
}

void Settings::setIsPostClosedMessage(const bool &on)
{
    m_isPostClosedMessage = on;
    s->setValue("IsPostClosedMessage", on);
}

void Settings::setIsPostUpdateNameSuccessedMessage(const bool &on)
{
    m_isPostUpdateNameSuccessedMessage = on;
    s->setValue("IsPostUpdateNameSuccessedMessage", on);
}

void Settings::setIsPostUpdateNameFailedMessage(const bool &on)
{
    m_isPostUpdateNameFailedMessage = on;
    s->setValue("IsPostUpdateNameFailedMessage", on);
}

void Settings::setIsPostUpdateUrlSuccessedMessage(const bool &on)
{
    m_isPostUpdateUrlSuccessedMessage = on;
    s->setValue("IsPostUpdateUrlSuccessedMessage", on);
}

void Settings::setIsPostUpdateUrlFailedMessage(const bool &on)
{
    m_isPostUpdateUrlFailedMessage = on;
    s->setValue("IsPostUpdateUrlFailedMessage", on);
}

void Settings::setIsPostUpdateLocationSuccessedMessage(const bool &on)
{
    m_isPostUpdateLocationSuccessedMessage = on;
    s->setValue("IsPostUpdateLocationSuccessedMessage", on);
}

void Settings::setIsPostUpdateLocationFailedMessage(const bool &on)
{
    m_isPostUpdateLocationFailedMessage = on;
    s->setValue("IsPostUpdateLocationFailedMessage", on);
}

void Settings::setIsPostUpdateDescriptionSuccessedMessage(const bool &on)
{
    m_isPostUpdateDescriptionSuccessedMessage = on;
    s->setValue("IsPostUpdateDescriptionSuccessedMessage", on);
}

void Settings::setIsPostUpdateDescriptionFailedMessage(const bool &on)
{
    m_isPostUpdateDescriptionFailedMessage = on;
    s->setValue("IsPostUpdateDescriptionFailedMessage", on);
}

void Settings::setIsPostUpdateImageSuccessedMessage(const bool &on)
{
    m_isPostUpdateImageSuccessedMessage = on;
    s->setValue("IsPostUpdateImageSuccessedMessage", on);
}

void Settings::setIsPostUpdateImageFailedMessage(const bool &on)
{
    m_isPostUpdateImageFailedMessage = on;
    s->setValue("IsPostUpdateImageFailedMessage", on);
}

void Settings::setIsStayOnSystemTray(const bool &on)
{
    m_isStayOnSystemTray = on;
    s->setValue("IsStayOnSystemTray", on);
}

void Settings::setIsAutoStartUpdateName(const bool &on)
{
    m_isAutoStartUpdateName = on;
    s->setValue("IsAutoStartUpdateName", on);
}
