#ifndef UPDATE_H
#define UPDATE_H

#include "twitter.h"
#include "settings.h"
#include "tweetobject.h"

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
    void executed(const QString &screen_name);
    void updated(const QString &updated_value);
    void error(const QString &error_string);
    void stateChanged(const Update::State);

protected:
    Twitter twitter;
    Settings settings;
    QString error_message;

protected slots:
    void recieveResult(const QString &message, const QString &in_reply_to_status_id = NULL);
};

#endif // UPDATE_H
