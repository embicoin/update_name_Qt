#ifndef UPDATE_H
#define UPDATE_H

#include "../restapi.h"
#include "../../object/tweets.h"
#include "../../object/error.h"

namespace TwitterAPI {
namespace Rest {
namespace Statuses {
struct UpdateParameters {
    explicit UpdateParameters();
    QString status;
    QString inReplyToStatusId;
    bool possiblySensitive = false;
    QString lat;
    QString _long;
    QString placeId;
    bool displayCoordinates = true;
    QStringList mediaIds;
};
class Update : public RestApi
{
    Q_OBJECT
public:    
    explicit Update(QObject *parent = 0);
    explicit Update(const OAuth &oauth, QObject *parent = 0);

signals:
    void finished();
    void successed(const TwitterAPI::Object::Tweets &tweet);
    void apiError(const TwitterAPI::Object::Error &error);
    void networkError(const QString &errorMessage);

public slots:
    TwitterAPI::Object::Tweets exec(const QString &status, const QString &inReplyToStatusId = QString::null,
                                    const bool possiblySensitive = false, const QString &lat = QString::null,
                                    const QString &_long = QString::null, const QString &placeId = QString::null,
                                    const bool &displayCoordinates = true, const QStringList &mediaIds = QStringList());
    TwitterAPI::Object::Tweets exec(const Statuses::UpdateParameters &parameters);

private:
    static const QUrl UPDATE_URL;
};
}
}
}

#endif // UPDATE_H
