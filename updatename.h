#ifndef UPDATENAME_H
#define UPDATENAME_H

#include "twitter.h"
#include "settings.h"

#include <QObject>

class UpdateName : public QObject
{
    Q_OBJECT
public:
    explicit UpdateName(QObject *parent = 0);

    enum State {
        PostStartupMessageFailed,
        PostClosedMessageFailed,
        GetScreenNameFailed,
        RecieveResultFailed,
        UpdateNameFailed,
        Aborted,
        Executed,
        NameUpdated,
        ResultRecieved,
        PostStartupMessageSuccessed,
        PostClosedMessageSuccessed,
        GetScreenNameSuccessed,
    };

    QString lastErrorMessage();
    QString lastExecutedUpdateNameUser();
    QString updatedName();

signals:
    void stateChanged(UpdateName::State state);
    void updateNameExecuted(QString &screen_name);
    void nameUpdated(QString &newName);

public slots:
    void postStartupMessage();
    void postClosedMessage();
    void exec(const QByteArray &twitter_status_object_json_data);

private:
    Twitter twitter;
    Settings settings;
    QString my_screen_name;
    QString error_message;
    QString executed_update_name_user;
    QString updated_name;

};

#endif // UPDATENAME_H
