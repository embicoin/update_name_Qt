#ifndef UPDATE_H
#define UPDATE_H

#include "../twitter/restclient.h"
#include "../twitter/tweetobject.h"
#include "../settings.h"

#include <QObject>

class Update : public QObject
{
    Q_OBJECT
public:
    explicit Update(QObject *parent = 0);

    enum State {
        Executed,
        Updated,
        ResultRecieved
    };

    enum ErrorState {
        UpdateFailed,
        ResultRecieveFailed,
    };

    UsersObject executedUser();
    QString errorString();

signals:
    void stateChanged(const Update::State&);
    void error(const Update::ErrorState&);
    void finished();

protected:
    RestClient m_twitter;
    Settings m_settings;
    UsersObject m_executedUser;
    QString m_errorMessage;

protected slots:
    void recieveResult(const QString &message, const QString &inReplyToStatusId = NULL);
};

#endif // UPDATE_H
