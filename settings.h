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
    QString userId() const;
    QString screenName() const;

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
    QString updateImageSuccessedMessage() const;
    QString updateImageFailedMessage() const;
    QString updateBackgroundImageSuccessedMessage() const;
    QString updateBackgroundImageFailedMessage() const;

    QString updateNameFormat() const;
    QString historyFileName() const;

    /* Is Enabled Update Commands */
    bool isEnabledUpdateName() const;
    bool isEnabledUpdateUrl() const;
    bool isEnabledUpdateLocation() const;
    bool isEnabledUpdateDescription() const;
    bool isEnabledUpdateImage() const;
    bool isEnabledUpdateBackgroundImage() const;

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
    bool isPostUpdateImageSuccessedMessage() const;
    bool isPostUpdateImageFailedMessage() const;
    bool isPostUpdateBackgroundSuccessedMessage() const;
    bool isPostUpdateBackgroundFailedMessage() const;

    bool isStayOnSystemTray() const;
    bool isAutoStartUpdateName() const;
    bool isRetryTweetOnStatusIsADuplicate() const;
    bool isWriteHistoryFile() const;

    /*Setter*/

    void setConsumerKey(const QString &ck);
    void setConsumerSecret(const QString &cs);
    void setAccessToken(const QString &at);
    void setAccessTokenSecret(const QString &ats);
    void setUserId(const QString &userId);
    void setScreenName(const QString &screenName);

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
    void setUpdateImageSuccessedMessage(const QString &message);
    void setUpdateImageFailedMessage(const QString &message);
    void setUpdateBackgroundImageSuccessedMessage(const QString &message);
    void setUpdateBackgroundImageFailedMessage(const QString &message);

    void setUpdateNameFormat(const QString &format);
    void setHistoryFileName(const QString &fileName);

    void setUpdateNameEnabled(const bool &enable);
    void setUpdateUrlEnabled(const bool &enable);
    void setUpdateLocationEnabled(const bool &enable);
    void setUpdateDescriptionEnabled(const bool &enable);
    void setUpdateImageEnabled(const bool &enable);
    void setUpdateBackgroundImageEnabled(const bool &enable);

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
    void setIsPostUpdateImageSuccessedMessage(const bool &on);
    void setIsPostUpdateImageFailedMessage(const bool &on);
    void setIsPostUpdateBackgroundImageSuccessedMessage(const bool &on);
    void setIsPostUpdateBackgroundImageFailedMessage(const bool &on);

    void setIsStayOnSystemTray(const bool &on);
    void setIsAutoStartUpdateName(const bool &on);
    void setIsRetryTweetOnStatusIsADuplicate(const bool &on);
    void setIsWriteHistoryFile(const bool &on);

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
    static const QString DEFAULT_UPDATE_IMAGE_SUCCESSED_MESSAGE;
    static const QString DEFAULT_UPDATE_IMAGE_FAILED_MESSAGE;
    static const QString DEFAULT_UPDATE_BACKGROUND_IMAGE_SUCCESSED_MESSAGE;
    static const QString DEFAULT_UPDATE_BACKGROUND_IMAGE_FAILED_MESSAGE;

    static QSettings *s;

    static QString m_consumerKey;
    static QString m_consumerSecret;
    static QString m_accessToken;
    static QString m_accessTokenSecret;
    static QString m_userId;
    static QString m_screenName;

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
    static QString m_updateImageSuccessedMessage;
    static QString m_updateImageFailedMessage;
    static QString m_updateBackgroundImageSuccessedMessage;
    static QString m_updateBackgroundImageFailedMessage;

    static QString m_updateNameFormat;
    static QString m_historyFileName;

    static bool m_isEnabledUpdateName;
    static bool m_isEnabledUdpateUrl;
    static bool m_isEnabledUpdateLocation;
    static bool m_isEnabledUpdateDescription;
    static bool m_isEnabledUpdateImage;
    static bool m_isEnabledUpdateBackgroundImage;

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
    static bool m_isPostUpdateImageSuccessedMessage;
    static bool m_isPostUpdateImageFailedMessage;
    static bool m_isPostUpdateBackgroundImageSuccessedMessage;
    static bool m_isPostUpdateBackgroundImageFailedMessage;

    static bool m_isStayOnSystemTray;
    static bool m_isAutoStartUpdateName;
    static bool m_isRetryTweetOnStatusIsADuplicate;
    static bool m_isWriteHisotryFile;
};

#endif // SETTINGS_H
