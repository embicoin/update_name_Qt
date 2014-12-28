#ifndef RESTAPI_H
#define RESTAPI_H

#include <QObject>
#include <QNetworkReply>
#include <QNetworkAccessManager>

#include "../oauth/oauth.h"
//#include "../twitter_global.h"

namespace TwitterAPI {
namespace Rest {
class RestApi : public QObject
{
    Q_OBJECT
public:
    explicit RestApi(QObject *parent = 0);
    explicit RestApi(const OAuth &oauth, QObject *parent = 0);

    void setOAuth(const OAuth &oauth);
    QByteArray requestApi(const QNetworkAccessManager::Operation &httpMethod, QUrl resourceUrl,
                          const QVariantMap &parameters = QVariantMap());
    QString errorString();
protected:
    QByteArray m_consumerKey;
    QByteArray m_consumerSecret;
    QByteArray m_accessToken;
    QByteArray m_accessTokenSecret;
    QString m_errorMessage;
};
}
}

#endif // RESTAPI_H
