#include "location.h"
#include "../updatehistory.h"

UpdateLocation::UpdateLocation(QObject *parent) :
    Update(parent)
{
}

QString UpdateLocation::location()
{
    return m_updatedLocation;
}

void UpdateLocation::exec(const TweetObject &tweet, const QString &newLocation)
{
    m_executedUser = tweet.user();
    emit stateChanged(Executed);

    qDebug() << "[Info] update_location: Update location to" << newLocation;

    try {
        m_updatedLocation = m_twitter.updateLocation(newLocation).location();
        emit stateChanged(Updated);

        qDebug() << "[Info] update_location: Location updated.\n"
                    "       New location:" << m_updatedLocation;

        if (m_settings.isPostUpdateLocationSuccessedMessage()) {
            recieveResult(m_settings.updateLocationSuccessedMessage()
                          .replace("%u", tweet.user().screen_name())
                          .replace("%l", m_updatedLocation), tweet.idStr());
        }

        if (m_settings.isWriteHistoryFile()) {
            UpdateHistory updateHistory;
            updateHistory.writeUpdateLocationHistory(tweet.user(), m_updatedLocation);
        }
    } catch (const std::runtime_error &e) {
        m_errorMessage = e.what();
        emit error(UpdateFailed);

        qCritical() << "[Error] update_location: Update location failed.\n"
                       "        Error message:" << m_errorMessage;

        if(m_settings.isPostUpdateLocationFailedMessage())
            recieveResult(m_settings.updateLocationFailedMessage()
                          .replace("%u", tweet.user().screen_name())
                          .replace("%l", newLocation)
                          .replace("%e", m_errorMessage), tweet.idStr());
        return;
    }
}
