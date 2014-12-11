#ifndef RESTCLIENT_H
#define RESTCLIENT_H

#include "../settings.h"
#include "usersobject.h"

#include <QObject>
#include <QNetworkAccessManager>
#include <QFile>

class RestClient : public QObject
{
    Q_OBJECT
public:
    explicit RestClient(QObject *parent = 0);

    UsersObject verifyCredentials();
    void statusUpdate(const QString &text,
                      const QString &in_reply_to_status_id = NULL,
                      const QStringList &media_ids = QStringList());
    UsersObject updateName(const QString &name);
    UsersObject updateUrl(const QString &url);
    UsersObject updateLocation(const QString &location);
    UsersObject updateDescroption(const QString &description);
    UsersObject updateProfile(const QString &name = NULL,
                        const QString &url = NULL,
                        const QString &location = NULL,
                        const QString &description = NULL);
    UsersObject updateProfileImage(const QByteArray &mediaData);
    UsersObject updateProfileBackground(const QByteArray &mediaData);
    UsersObject usersLookup(const QString &screenName = NULL, const QString &userId = NULL);
    QString mediaUpload(const QString &mediaFileName);
    QString mediaUpload(const QByteArray &mediaData);

private:
    QByteArray requestTwitterApi(const QNetworkAccessManager::Operation method,
                                 const QString &url,
                                 const QVariantMap &dataParams = QVariantMap());

    static const QString ACCOUNT_VERIFY_CREDENTIALS_URL;
    static const QString ACCOUNT_UPDATE_PROFILE_URL;
    static const QString STATUSES_UPDATE_URL;
    static const QString MEDIA_UPLOAD_URL;
    static const QString ACCOUT_UPDATE_PROFILE_IMAGE_URL;
    static const QString USERS_LOOKUP_URL;
    static const QString ACCOUNT_UPDATE_PROFILE_BACKGROUND_IMAGE_URL;
    Settings m_settings;

};

#endif // RESTCLIENT_H
