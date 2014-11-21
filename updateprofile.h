#ifndef UPDATEPROFILE_H
#define UPDATEPROFILE_H

#include "twitter/restclient.h"
#include "settings.h"
#include "update/name.h"
#include "update/url.h"
#include "update/location.h"
#include "update/description.h"

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

    QString updateNameErrorString();

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
    RestClient m_twitter;
    Settings m_settings;

    UpdateName m_updatename;
    UpdateUrl m_updateurl;
    UpdateLocation m_updatelocation;
    UpdateDescription m_udpatedescription;

    QString m_profilevalue;
    QString m_myscreenname;
    QString m_errormessage;
    QString m_executeduser;
};

#endif // UPDATEPROFILE_H
