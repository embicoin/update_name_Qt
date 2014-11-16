#ifndef TWITTER_H
#define TWITTER_H

#include "settings.h"

#include <QObject>
#include <QNetworkAccessManager>
#include <QFile>

class Twitter : public QObject
{
    Q_OBJECT
public:
    explicit Twitter(QObject *parent = 0);

    QString getScreenName();
    QString getName();
    void statusUpdate(const QString &text,
                      const QString &in_reply_to_status_id = NULL,
                      const QStringList &media_ids = QStringList());
    void updateName(const QString &name);
    void updateUrl(const QString &url);
    void updateLocation(const QString &location);
    void updateDescroption(const QString &description);
    void updateProfile(const QString &name = NULL,
                        const QString &url = NULL,
                        const QString &location = NULL,
                        const QString &description = NULL);
    QString mediaUpload(const QString &media_file_name);

private:
    QByteArray requestTwitterApi(const QNetworkAccessManager::Operation method,
                                 const QString &url,
                                 const QVariantMap &dataParams = QVariantMap());

    static const QString ACCOUNT_VERIFY_CREDENTIALS_URL;
    static const QString ACCOUNT_UPDATE_PROFILE_URL;
    static const QString STATUSES_UPDATE_URL;
    static const QString MEDIA_UPLOAD_URL;

    Settings settings;

};

#endif // TWITTER_H
