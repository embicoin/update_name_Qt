#ifndef UPDATEHISTORY_H
#define UPDATEHISTORY_H

#include "twitter/usersobject.h"
#include <QJsonObject>

class UpdateHistory
{
public:    
    UpdateHistory();

    void setHistoryFileName(const QString &fileName);
    bool writeUpdateNameHistory(const UsersObject &executedUser, const QString &newName);
    bool writeUpdateUrlHistory(const UsersObject &executedUser, const QUrl &newUrl);
    bool writeUpdateLocationHistory(const UsersObject &executedUser, const QString &newLocation);
    bool writeUpdateDescriptionHistory(const UsersObject &executedUser, const QString &newDescription);
    bool writeUpdateImageHistory(const UsersObject &executedUser);
    bool writeUpdateBackgroundImageHistory(const UsersObject &executedUser);

    QString historyFileName();

private:
    bool writeHistory(const QJsonObject &object);

    QString m_historyFileName;
};

#endif // UPDATEHISTORY_H
