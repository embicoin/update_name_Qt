#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>
#include <QObject>

class Settings
{
public:
    explicit Settings();
    ~Settings();

    QString filePath() const;

    /*Getter*/

    QString consumerKey() const;
    QString consumerSecret() const;
    QString accessToken() const;
    QString accessTokenSecret() const;

    /* Message */
    QString startupMessage() const;
    QString closedMessage() const;
    QString updateNameSuccessedMessage() const;
    QString updateNameFailedMessage() const;
    QString updateUrlSuccessedMessage() const;
    QString updateUrlFailedMessage() const;
    QString updateLocationSuccessedMessage() const;
    QString updateLocationFailedMessage() const;
    QString updateDescriptionSuccessedMessage() const;
    QString updateDescriptionFailedMessage() const;

    /* Is Enabled Update Commands */
    bool isEnabledUpdateName() const;
    bool isEnabledUpdateUrl() const;
    bool isEnabledUpdateLocation() const;
    bool isEnabledUpdateDescription() const;

    /* Is Post Messages */
    bool isPostStartupMessage() const;
    bool isPostClosedMessage() const;
    bool isPostUpdateNameSuccessedMessage() const;
    bool isPostUpdateNameFailedMessage() const;
    bool isPostUpdateUrlSuccessedMessage() const;
    bool isPostUpdateUrlFailedMessage() const;
    bool isPostUpdateLocationSuccessedMessage() const;
    bool isPostUpdateLocationFailedMessage() const;
    bool isPostUpdateDescriptionSuccessedMessage() const;
    bool isPostUpdateDescriptionFailedMessage() const;

    bool isStayOnSystemTray() const;
    bool isAutoStartUpdateName() const;

    /*Setter*/

    void setConsumerKey(const QString &ck);
    void setConsumerSecret(const QString &cs);
    void setAccessToken(const QString &at);
    void setAccessTokenSecret(const QString &ats);

    void setStartupMessage(const QString &message);
    void setClosedMessage(const QString &message);
    void setUpdateNameSuccessedMessage(const QString &message);
    void setUpdateNameFailedMessage(const QString &message);
    void setUpdateUrlSuccessedMessage(const QString &message);
    void setUpdateUrlFailedMessage(const QString &message);
    void setUpdateLocationSuccessedMessage(const QString &message);
    void setUpdateLocationFailedMessage(const QString &message);
    void setUpdateDescriptionSuccessedMessage(const QString &message);
    void setUpdateDescriptionFailedMessage(const QString &message);

    void setUpdateNameEnabled(const bool &enable);
    void setUpdateUrlEnabled(const bool &enable);
    void setUpdateLocationEnabled(const bool &enable);
    void setUpdateDescriptionEnabled(const bool &enable);

    void setIsPostStartupMessage(const bool &on);
    void setIsPostClosedMessage(const bool &on);
    void setIsPostUpdateNameSuccessedMessage(const bool &on);
    void setIsPostUpdateNameFailedMessage(const bool &on);
    void setIsPostUpdateUrlSuccessedMessage(const bool &on);
    void setIsPostUpdateUrlFailedMessage(const bool &on);
    void setIsPostUpdateLocationSuccessedMessage(const bool &on);
    void setIsPostUpdateLocationFailedMessage(const bool &on);
    void setIsPostUpdateDescriptionSuccessedMessage(const bool &on);
    void setIsPostUpdateDescriptionFailedMessage(const bool &on);

    void setIsStayOnSystemTray(const bool &on);
    void setIsAutoStartUpdateName(const bool &on);

private:
    static const QString SETTINGS_FILE_NAME;

    /*Default Settings*/
    static const QString DEFAULT_STARTUP_MESSAGE;
    static const QString DEFAULT_CLOSED_MESSAGE;
    static const QString DEFAULT_UPDATE_NAME_SUCCESSED_MESSAGE;
    static const QString DEFAULT_UPDATE_NAME_FAILED_MESSAGE;
    static const QString DEFAULT_UPDATE_URL_SUCCESSED_MESSAGE;
    static const QString DEFAULT_UPDATE_URL_FAILED_MESSAGE;
    static const QString DEFAULT_UPDATE_LOCATION_SUCCESSED_MESSAGE;
    static const QString DEFAULT_UPDATE_LOCATION_FAILED_MESSAGE;
    static const QString DEFAULT_UPDATE_DESCRIPTION_SUCCESSED_MESSAGE;
    static const QString DEFAULT_UPDATE_DESCRIPTION_FAILED_MESSAGE;

    static QSettings *s;

    static QString consumer_key;
    static QString consumer_secret;
    static QString access_token;
    static QString access_token_secret;

    static QString startup_message;
    static QString closed_message;
    static QString update_name_successed_message;
    static QString update_name_failed_message;
    static QString update_url_successed_message;
    static QString update_url_failed_message;
    static QString update_location_successed_message;
    static QString update_location_failed_message;
    static QString update_description_successed_message;
    static QString update_description_failed_message;

    static bool is_enabled_update_name;
    static bool is_enabled_udpate_url;
    static bool is_enabled_update_location;
    static bool is_enabled_update_description;

    static bool is_post_startup_message;
    static bool is_post_closed_message;
    static bool is_post_update_name_successed_message;
    static bool is_post_update_name_failed_message;
    static bool is_post_update_url_successed_message;
    static bool is_post_update_url_failed_message;
    static bool is_post_update_location_successed_message;
    static bool is_post_update_location_failed_message;
    static bool is_post_update_description_successed_message;
    static bool is_post_update_description_failed_message;

    static bool is_stay_on_system_tray;
    static bool is_auto_start_update_name;
};

#endif // SETTINGS_H
