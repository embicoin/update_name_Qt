#include "updatehistory.h"
#include "settings.h"

#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>
#include <QFile>
#include <QStringList>
#include <QDateTime>

UpdateHistory::UpdateHistory()
{
}

bool UpdateHistory::writeUpdateNameHistory(const UsersObject &executedUser, const QString &newName)
{
    const QJsonObject nameObj
    {
        {"Executed user's screen_name", executedUser.screen_name()},
        {"New name", newName}
    };
    const QJsonObject obj
    {
        {"update_name", nameObj}
    };
    return writeHistory(obj);
}

bool UpdateHistory::writeUpdateUrlHistory(const UsersObject &executedUser, const QUrl &newUrl)
{
    const QJsonObject urlObj
    {
        {"Executed user's screen_name", executedUser.screen_name()},
        {"New url", newUrl.toString()}
    };
    const QJsonObject obj
    {
        {"update_url", urlObj}
    };
    return writeHistory(obj);
}

bool UpdateHistory::writeUpdateLocationHistory(const UsersObject &executedUser, const QString &newLocation)
{
    const QJsonObject locationObj
    {
        {"Executed user's screen_name", executedUser.screen_name()},
        {"New location", newLocation}
    };
    const QJsonObject obj
    {
        {"update_location", locationObj}
    };
    return writeHistory(obj);
}

bool UpdateHistory::writeUpdateDescriptionHistory(const UsersObject &executedUser, const QString &newDescription)
{
    const QJsonObject descriptionObj
    {
        {"Executed user's screen_name", executedUser.screen_name()},
        {"New description", newDescription}
    };
    const QJsonObject obj
    {
        {"update_description", descriptionObj}
    };
    return writeHistory(obj);
}

bool UpdateHistory::writeUpdateImageHistory(const UsersObject &executedUser)
{
    const QJsonObject imageObj
    {
        {"Executed user's screen_name", executedUser.screen_name()}
    };
    const QJsonObject obj
    {
        {"update_image", imageObj}
    };
    return writeHistory(obj);
}

bool UpdateHistory::writeUpdateBackgroundImageHistory(const UsersObject &executedUser)
{
    const QJsonObject backgroundImageObj
    {
        {"Executed user's screen_name", executedUser.screen_name()}
    };
    const QJsonObject obj
    {
        {"update_background_image", backgroundImageObj}
    };
    return writeHistory(obj);
}

bool UpdateHistory::writeUpdateBannerHistory(const UsersObject &executedUser)
{
    const QJsonObject bannerObj
    {
        {"Executed user's screen_name", executedUser.screen_name()}
    };
    const QJsonObject obj
    {
        {"update_background_image", bannerObj}
    };
    return writeHistory(obj);
}

bool UpdateHistory::writeHistory(QJsonObject object)
{
    if (m_settings.historyFileName().isEmpty())
        return false;

    QFile historyFile(m_settings.historyFileName());
    QJsonDocument doc;
    QJsonArray array;

    object.insert("Date time", QDateTime::currentDateTime().toString());

    if (historyFile.exists()) {
        if (!historyFile.open(QFile::ReadOnly | QFile::Text))
            return false;
        array = QJsonDocument::fromJson(historyFile.readAll()).array();
        historyFile.close();
    }
    array << object;
    doc.setArray(array);
    if (!historyFile.open(QFile::WriteOnly | QFile::Text))
        return false;
    return historyFile.write(doc.toJson());
}
