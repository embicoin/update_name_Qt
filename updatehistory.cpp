#include "updatehistory.h"

#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>
#include <QFile>
#include <QStringList>

UpdateHistory::UpdateHistory()
{
}

void UpdateHistory::setHistoryFileName(const QString &fileName)
{
    m_historyFileName = fileName;
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

bool UpdateHistory::writeHistory(const QJsonObject &object)
{
    if (m_historyFileName.isEmpty() || !QFile::exists(m_historyFileName))
        return false;

    QFile historyFile(m_historyFileName);

    if (!historyFile.open(QFile::ReadWrite))
        return false;

    QJsonArray array = QJsonDocument::fromJson(historyFile.readAll()).array();
    array << object;
    QJsonDocument doc(array);
    return historyFile.write(doc.toJson());
}
