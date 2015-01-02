#ifndef UPDATENAME_H
#define UPDATENAME_H

#include "twitterAPI/streaming/user.h"
#include "twitterAPI/rest/statuses/update.h"

#include <QThread>

class UpdateProfile : public QObject
{
    Q_OBJECT
public:
    explicit UpdateProfile(const TwitterAPI::OAuth &oauth, QObject *parent = 0);

    QString errorString() const;

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
    void updateStarted(UpdateProfile::UpdateType type, const TwitterAPI::Object::Users &executedUser);
    void updateFinished(UpdateProfile::UpdateType type, const QString &newProfile);
    void resultPosted();
    void updateError(UpdateProfile::UpdateType type, const QString &errorMessage);
    void resultPostError(const QString &errorMessage);
    void finished();

public slots:
    void update(UpdateProfile::UpdateType type, const TwitterAPI::Object::Tweets &updateTweet, const QString newProfile);

private:
    void postResult(const TwitterAPI::Rest::Statuses::UpdateParameters &parameters);

    TwitterAPI::OAuth m_oauth;
    QString m_errorMessage;
};

class UpdateName : public QThread
{
    Q_OBJECT
public:
    UpdateName(QObject *parent = 0);
    ~UpdateName();

    enum State {
        UserStream,
        ScreenNameLookup,
    };

signals:
    void started();
    void running();
    void waitting(uint waitCount);
    void stopping();
    void screenNameLookuped(const QString &screenName);
    void error(UpdateName::State state, const QString &errorMessage);

    void updateStarted(UpdateProfile::UpdateType type, const TwitterAPI::Object::Users &executedUser);
    void updateFinished(UpdateProfile::UpdateType type, const QString &newProfile);
    void resultPosted();
    void updateError(UpdateProfile::UpdateType type, const QString &errorMessage);
    void resultPostError(const QString &errorMessage);

public slots:
    void stop();

private slots:
    void startUpdateName(const TwitterAPI::Object::Tweets &tweet);

private:
    void run();

    TwitterAPI::Streaming::User *m_userStream;
    TwitterAPI::OAuth m_oauth;
    QString m_screenName;
};

#endif // UPDATENAME_H
