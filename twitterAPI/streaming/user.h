#ifndef USER_H
#define USER_H

#include "../oauth/oauth.h"
#include "../object/tweets.h"
#include "parsejson.h"

#include <QThread>

namespace TwitterAPI {
namespace Streaming {
struct UserParameters {
    explicit UserParameters();
    QString delimited;
    bool stallWarnings = false;
    QString with;
    QString replies;
    QStringList track;
    QList<double> locations;
    bool stringifyFriendIds;
};

class User : public QThread
{
    Q_OBJECT
public:
    explicit User(const TwitterAPI::OAuth &oauth, QObject *parent = 0);
    ~User();

public slots:
    void stop();

signals:
    void started();
    void running();
    void disConnected();
    void stopping();
    void error(const QString &errorMessage);
    void wait(uint count);
    void recievedTweet(const TwitterAPI::Object::Tweets &tweet);
    void recievedStatusDeletion(const TwitterAPI::Streaming::StatusDeletionNotices &notice);

private:
    void run();

    static const QUrl USERSTREAM_URL;

    volatile bool m_stopped = true; //ユーザーストリームが止まっているかどうか
    QByteArray m_consumerKey;
    QByteArray m_consumerSecret;
    QByteArray m_accessToken;
    QByteArray m_accessTokenSecret;
};
}
}

#endif // USER_H
