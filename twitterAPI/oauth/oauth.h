#ifndef OAUTH_H
#define OAUTH_H

#include <QObject>
#include <QUrl>
#include <QNetworkReply>

class OAuth : public QObject
{
    Q_OBJECT
public:
    explicit OAuth(QObject *parent = 0);
    explicit OAuth(const QString &consumerKey, const QString &consumerSecret, QObject *parent = 0);
    explicit OAuth(const QString &consumerKey, const QString &consumerSecret,
                   const QString &accessToken, const QString &accessTokenSecret, QObject *parent = 0);

    QString consumerKey() const;
    QString consumerSecret() const;
    QString accessToken() const;
    QString accessTokenSecret() const;
    QString userId() const;
    QString screenName() const;

    void setConsumerKey(const QString &consumerKey);
    void setConsumerSecret(const QString &consumerSecret);
    void setAccessToken(const QString &accessToken);
    void setAccessTokenSecret(const QString &accessTokenSecret);

signals:
    void requestTokenFinished();
    void requestTokenRecieved(const QUrl &authenticateUrl);
    void requestTokenError(const QNetworkReply::NetworkError &error, const QString &errorResponse);

    void authorizeFinished();
    void authorizeSuccessed(const QString &accessToken, const QString &accessTokenSecret,
                            const QString &userId, const QString &screenName);
    void authorizeError(const QNetworkReply::NetworkError &error, const QString &errorResponse);

public slots:
    QUrl authorizeUrl();
    void authorizePin(const QString &pinCode);

private:
    void initialize();

    static const QUrl REQUEST_TOKEN_URL;
    static const QUrl AUTHORIZE_URL;
    static const QUrl ACCESS_TOKEN_URL;

    QString m_consumerKey;
    QString m_consumerSecret;
    QString m_requestToken;
    QString m_requestTokenSecret;
    QString m_accessToken;
    QString m_accessTokenSecret;
    QString m_userId;
    QString m_screenName;
};

#endif // OAUTH_H
