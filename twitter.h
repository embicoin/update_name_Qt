#ifndef TWITTER_H
#define TWITTER_H

#include "settings.h"

#include <QObject>
#include <QNetworkAccessManager>

class Twitter : public QObject
{
    Q_OBJECT
public:
    explicit Twitter(QObject *parent = 0);

    QString getScreenName();
    QString getName();
    void statusUpdate(const QString &text, const QString &in_reply_to_status_id = NULL);
    void updateName(const QString &name);

private:
    QByteArray requestTwitterApi(const QNetworkAccessManager::Operation method,
                                 const QString &url,
                                 const QVariantMap &dataParams = QVariantMap());

    static const QString ACCOUNT_VERIFY_CREDENTIALS_URL;
    static const QString ACCOUNT_UPDATE_PROFILE_URL;
    static const QString STATUSES_UPDATE_URL;

    Settings settings;

};

#endif // TWITTER_H
