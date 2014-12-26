#ifndef PARSEJSON_H
#define PARSEJSON_H

#include "../object/tweets.h"

#include <QThread>
#include <QJsonDocument>

namespace TwitterAPI {
namespace Streaming {

struct StatusDeletionNotices {
    explicit StatusDeletionNotices();
    qint64 id;
    QString idStr;
    qint64 userId;
    QString userIdStr;
};

class ParseJson : public QThread
{
    Q_OBJECT
public:
    explicit ParseJson(const QByteArray &json, QObject *parent = 0);

signals:
    void tweet(const TwitterAPI::Object::Tweets &tweet);
    void statusDeletion(const TwitterAPI::Streaming::StatusDeletionNotices &notice);

private:
    void run();

    QJsonDocument m_json;
};
}
}

#endif // PARSEJSON_H
