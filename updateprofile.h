#ifndef UPDATEPROFILE_H
#define UPDATEPROFILE_H

#include "twitter/restclient.h"
#include "settings.h"
#include "update/name.h"

#include <QObject>

class UpdateProfile : public QObject
{
    Q_OBJECT
public:
    explicit UpdateProfile(QObject *parent = 0);

    enum ProfileType {
        Name,
        Url,
        Location,
        Description
    };

    QString executedUserScreenName();
    QString profileValue();
    QString errorString();
    //QString url();
    //QString location();
    //QString description();

    QString updateNameErrorString();
    //QString updateUrlErrorString();
    //QString updateLocationErrorString();
    //QString updateDescriptionErrorString();

signals:
    void stateChanged(const Update::State &state, const UpdateProfile::ProfileType &type);

    void updateNameStateChanged(const Update::State &state);
    void nameUpdated(const QString &newName);

    /*
    void urlUpdated(const QString &newUrl);
    void locationUpdated(const QString &newLocation);
    void descriptionUpdated(const QString &newDescription);
    */

public slots:
    void postStartupMessage();
    void postClosedMessage();
    void exec(const QByteArray &twitter_status_object_json_data);
    
private:
    RestClient twitter;
    Settings settings;

    UpdateName update_name;

    QString profile_value;
    QString my_screen_name;
    QString error_message;
    QString executed_user;
    QString udpate_tweet_id;
    QString updated_name;

};

#endif // UPDATEPROFILE_H
