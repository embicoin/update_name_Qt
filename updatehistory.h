#ifndef UPDATEHISTORY_H
#define UPDATEHISTORY_H

#include "twitter/usersobject.h"
#include "settings.h"
#include <QJsonObject>

class UpdateHistory
{
public:    
    UpdateHistory();

    bool writeUpdateNameHistory(const UsersObject &executedUser, const QString &newName);
    bool writeUpdateUrlHistory(const UsersObject &executedUser, const QUrl &newUrl);
    bool writeUpdateLocationHistory(const UsersObject &executedUser, const QString &newLocation);
    bool writeUpdateDescriptionHistory(const UsersObject &executedUser, const QString &newDescription);
    bool writeUpdateImageHistory(const UsersObject &executedUser);
    bool writeUpdateBackgroundImageHistory(const UsersObject &executedUser);
    bool writeUpdateBannerHistory(const UsersObject &executedUser);

private:
    bool writeHistory(QJsonObject object);

    QString m_historyFileName;
    Settings m_settings;
};

#endif // UPDATEHISTORY_H
