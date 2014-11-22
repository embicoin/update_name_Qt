#include "description.h"

UpdateDescription::UpdateDescription(Update *parent) :
    Update(parent)
{
}

QString UpdateDescription::description()
{
    return m_updatedDescription;
}

void UpdateDescription::exec(const TweetObject &tweet, const QString &newDescription)
{
    if(tweet.text().isEmpty()) {
        return;
    }

    emit stateChanged(Executed);

    try {
        m_twitter.updateDescroption(newDescription);
    } catch(const std::runtime_error &e) {
        m_errorMessage = QString::fromStdString(e.what());
        emit error(m_errorMessage);
        emit stateChanged(UpdateFailed);

        if(m_settings.isPostUpdateDescriptionFailedMessage()) {
            recieveResult(m_settings.updateDescriptionFailedMessage()
                          .replace("%u", tweet.user().screen_name())
                          .replace("%d", newDescription)
                          .replace("%e", m_errorMessage));
        }
        return;
    }

    try {
        m_updatedDescription = m_twitter.verifyCredentials().description();
    } catch(...) {
        m_updatedDescription = newDescription;
    }

    emit stateChanged(UpdateSuccessed);
    emit updated(m_updatedDescription);

    if(m_settings.isPostUpdateDescriptionSuccessedMessage()) {
        recieveResult(m_settings.updateDescriptionSuccessedMessage()
                      .replace("%u", tweet.user().screen_name())
                      .replace("%d", m_updatedDescription), tweet.idStr());
    }
}
