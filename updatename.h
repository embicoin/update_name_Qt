#ifndef UPDATENAME_H
#define UPDATENAME_H

#include "twitterAPI/streaming/user.h"
#include "twitterAPI/rest/statuses/update.h"

#include <QThread>

class UpdateName : public QThread
{
    Q_OBJECT
public:
    UpdateName(QObject *parent = 0);
    ~UpdateName();

    enum State {
        UserStream,
        ScreenNameLookup,
        PostResultTweet,
    };
    enum UpdateType {
        Name,
        Url,
        Location,
        Description,
        Image,
        Background,
        Banner,
    };

signals:
    void started();
    void running();
    void stopping();
    void stopped();
    void error(UpdateName::State state, const QString &errorMessage);
    void updateError(UpdateName::UpdateType type, const QString &errorMessage);
    void screenNameLookuped(const QString &errorMessage);
    void updated(UpdateName::UpdateType type, const QString &newProfile);
    void resultPosted();

private slots:
    void postResult(const TwitterAPI::Rest::Statuses::UpdateParameters &parameters);

private:
    void run();

    TwitterAPI::Streaming::User *m_userStream;
    TwitterAPI::OAuth m_oauth;
    QString m_screenName;
};

#endif // UPDATENAME_H
