#include "location.h"

UpdateLocation::UpdateLocation(Update *parent) :
    Update(parent)
{
}

QString UpdateLocation::location()
{
    return m_updatedLocation;
}

void UpdateLocation::exec(const TweetObject &tweet, const QString &newLocation)
{
    if(newLocation.isEmpty()) {
        return;
    }

    emit executed(tweet.user());

    try {
        m_updatedLocation = m_twitter.updateLocation(newLocation).location();
        emit updated(m_updatedLocation);
    } catch(const std::runtime_error &e) {
        m_errorMessage = e.what();
        emit error(UpdateProfile, m_errorMessage);

        if(m_settings.isPostUpdateLocationFailedMessage())
            recieveResult(m_settings.updateLocationFailedMessage()
                          .replace("%u", tweet.user().screen_name())
                          .replace("%l", newLocation)
                          .replace("%e", m_errorMessage), tweet.idStr());
        return;
    }

    if(m_settings.isPostUpdateLocationSuccessedMessage()) {
        recieveResult(m_settings.updateLocationSuccessedMessage()
                      .replace("%u", tweet.user().screen_name())
                      .replace("%l", m_updatedLocation), tweet.idStr());
    }
}
