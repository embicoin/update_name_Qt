#ifndef UPDATEPROFILE_H
#define UPDATEPROFILE_H

#include "twitter/restclient.h"
#include "update/name.h"
#include "update/url.h"
#include "update/location.h"
#include "update/description.h"
#include "update/image.h"
#include "settings.h"

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
        Description,
        Image
    };

    enum State {
        Aborted,
        UpdateSuccessed,
        UpdateFailed,
        RecieveResultSuccessed,
        RecieveResultFailed,
    };

    QString executedUserScreenName();
    QString profileValue();
    QString errorString();

    QString updateNameErrorString();

    static QString profileTypeString(const UpdateProfile::ProfileType&);

signals:
    void executed(const ProfileType, const UsersObject &excutedUser);
    void aborted(const ProfileType);
    void updated(const ProfileType, const QString &updatedProfileValue);
    void resultRecieved(const ProfileType);
    void error(const UpdateProfile::ProfileType, const Update::State, const QString &errorMessage);
    void updateNameStateChanged(const Update::State);
    void finished();

    /*
    void urlUpdated(const QString &newUrl);
    void locationUpdated(const QString &newLocation);
    void descriptionUpdated(const QString &newDescription);
    */

public slots:
    void postStartupMessage();
    void postClosedMessage();
    bool exec(const QByteArray &twitter_status_object_json_data);
    
private:
    RestClient m_twitter;
    Settings m_settings;

    UpdateName m_updateName;
    UpdateUrl m_updateUrl;
    UpdateLocation m_updateLocation;
    UpdateDescription m_updateDescription;
    UpdateImage m_updateImage;

    QString m_profilevalue;
    QString m_myscreenname;
    QString m_errormessage;
    QString m_executeduser;

    QStringList updateCommands = QStringList() << tr("name")
                                               << tr("url")
                                               << tr("location")
                                               << tr("description")
                                               << tr("image");
};

#endif // UPDATEPROFILE_H
