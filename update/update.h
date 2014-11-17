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
    void updated(const QString &updated_value);
    void error(const QString &error_string);
    void stateChanged(const Update::State);

protected:
    RestClient twitter;
    Settings settings;
    QString error_message;

protected slots:
    void recieveResult(const QString &message, const QString &in_reply_to_status_id = NULL);
};

#endif // UPDATE_H
