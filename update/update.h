#ifndef UPDATE_H
#define UPDATE_H

#include "twitter/restclient.h"
#include "twitter/tweetobject.h"
#include "settings.h"

#include <QObject>

class Update : public QObject
{
    Q_OBJECT
public:
    explicit Update(QObject *parent = 0);

    enum State {
        Executed,
        Aborted,
        UpdateSuccessed,
        UpdateFailed,
        RecieveResultSuccessed,
        RecieveResultFailed,
    };

    QString errorString();

signals:
    void updated(const QString &updatedValue);
    void error(const QString &errorString);
    void stateChanged(const Update::State);

protected:
    RestClient m_twitter;
    Settings m_settings;
    QString m_errorMessage;

protected slots:
    void recieveResult(const QString &message, const QString &inReplyToStatusId = NULL);
};

#endif // UPDATE_H
