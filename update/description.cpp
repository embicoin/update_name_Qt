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

    emit executed(tweet.user());

    try {
        m_updatedDescription = m_twitter.updateDescroption(newDescription).description();
        emit updated(m_updatedDescription);
    } catch(const std::runtime_error &e) {
        m_errorMessage = QString::fromStdString(e.what());
        emit error(ResultRecieve, m_errorMessage);

        if(m_settings.isPostUpdateDescriptionFailedMessage()) {
            recieveResult(m_settings.updateDescriptionFailedMessage()
                          .replace("%u", tweet.user().screen_name())
                          .replace("%d", newDescription)
                          .replace("%e", m_errorMessage));
        }
        return;
    }

    if(m_settings.isPostUpdateDescriptionSuccessedMessage()) {
        recieveResult(m_settings.updateDescriptionSuccessedMessage()
                      .replace("%u", tweet.user().screen_name())
                      .replace("%d", m_updatedDescription), tweet.idStr());
    }
}
