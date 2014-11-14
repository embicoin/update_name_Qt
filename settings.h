#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>
#include <QObject>

class Settings
{
public:
    explicit Settings();
    ~Settings();

    QString filePath();

    QString consumerKey();
    QString consumerSecret();
    QString accessToken();
    QString accessTokenSecret();
    QString startupMessage();
    QString closedMessage();
    QString updateNameSuccessedMessage();
    QString updateNameFailedMessage();
    bool isPostStartupMessage();
    bool isPostClosedMessage();
    bool isPostUpdateNameSuccessedMessage();
    bool isPostUpdateNameFailedMessage();
    bool isStayOnSystemTray();
    bool isAutoStartUpdateName();

    void setConsumerKey(const QString &ck);
    void setConsumerSecret(const QString &cs);
    void setAccessToken(const QString &at);
    void setAccessTokenSecret(const QString &ats);
    void setStartupMessage(const QString &message);
    void setClosedMessage(const QString &message);
    void setUpdateNameSuccessedMessage(const QString &message);
    void setUpdateNameFailedMessage(const QString &message);
    void setIsPostStartupMessage(const bool on);
    void setIsPostClosedMessage(const bool on);
    void setIsPostUpdateNameSuccessedMessage(const bool on);
    void setIsPostUpdateNameFailedMessage(const bool on);
    void setIsStayOnSystemTray(const bool on);
    void setIsAutoStartUpdateName(const bool on);

private:
    static const QString SETTINGS_FILE_NAME;
    static const QString DEFAULT_STARTUP_MESSAGE;
    static const QString DEFAULT_CLOSED_MESSAGE;
    static const QString DEFAULT_UPDATE_NAME_SUCCESSED_MESSAGE;
    static const QString DEFAULT_UPDATE_NAME_FAILED_MESSAGE;

    static QSettings *s;

    static QString consumer_key;
    static QString consumer_secret;
    static QString access_token;
    static QString access_token_secret;
    static QString startup_message;
    static QString closed_message;
    static QString update_name_successed_message;
    static QString update_name_failed_message;
    static bool is_post_startup_message;
    static bool is_post_closed_message;
    static bool is_post_udpate_name_successed_message;
    static bool is_post_update_name_failed_message;
    static bool is_stay_on_system_tray;
    static bool is_auto_start_update_name;
};

#endif // SETTINGS_H
