#ifndef UPDATEPROFILE_H
#define UPDATEPROFILE_H

#include "twitter/restclient.h"
#include "settings.h"
#include "update/update.h"

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
        Image,
        BackgroundImage,
        Banner,
    };
    enum State {
        GetScreenNameFinished,
    };
    enum ErrorState {
        GetScreenNameFailed,
    };

    static QString profileTypeString(const UpdateProfile::ProfileType&);

    QString screenName();
    QString errorString();

signals:
    void executed(const UpdateProfile::ProfileType &, const UsersObject &excutedUser);
    void aborted(const UpdateProfile::ProfileType &);
    void updated(const UpdateProfile::ProfileType &, const QString &updatedProfileValue);
    void resultRecieved(const UpdateProfile::ProfileType &);
    void updateError(const UpdateProfile::ProfileType &, const Update::ErrorState, const QString &errorMessage);
    void stateChanged(const UpdateProfile::State &);
    void error(const UpdateProfile::ErrorState &);
    void finished();

public slots:
    void postStartupMessage();
    void postClosedMessage();
    void exec(const QByteArray &twitter_status_object_json_data);

private slots:
    bool getScreenName();
    
private:
    RestClient m_twitter;
    Settings m_settings;

    static QString m_myscreenname;
    QString m_profilevalue;
    QString m_errormessage;
    QString m_executeduser;

    static const QStringList updateCommands;
};

#endif // UPDATEPROFILE_H
