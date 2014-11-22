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

    static QString m_consumerKey;
    static QString m_consumerSecret;
    static QString m_accessToken;
    static QString m_accessToken_secret;

    static QString m_startupMessage;
    static QString m_closedMessage;
    static QString m_updateNameSuccessedMessage;
    static QString m_updateNameFailedMessage;
    static QString m_updateUrlSuccessedMessage;
    static QString m_updateUrlFailedMessage;
    static QString m_updateLocationSuccessedMessage;
    static QString m_updateLocationFailedMessage;
    static QString m_updateDescriptionSuccessedMessage;
    static QString m_updateDescriptionFailedMessage;

    static bool m_isEnabledUpdateName;
    static bool m_isEnabledUdpateUrl;
    static bool m_isEnabledUpdateLocation;
    static bool m_isEnabledUpdateDescription;

    static bool m_isPostStartupMessage;
    static bool m_isPostClosedMessage;
    static bool m_isPostUpdateNameSuccessedMessage;
    static bool m_isPostUpdateNameFailedMessage;
    static bool m_isPostUpdateUrlSuccessedMessage;
    static bool m_isPostUpdateUrlFailedMessage;
    static bool m_isPostUpdateLocationSuccessedMessage;
    static bool m_isPostUpdateLocationFailedMessage;
    static bool m_isPostUpdateDescriptionSuccessedMessage;
    static bool m_isPostUpdateDescriptionFailedMessage;

    static bool m_isStayOnSystemTray;
    static bool m_isAutoStartUpdateName;
};

#endif // SETTINGS_H
