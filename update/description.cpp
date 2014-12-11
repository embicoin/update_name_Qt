#include "description.h"
#include "../updatehistory.h"

UpdateDescription::UpdateDescription(QObject *parent) :
    Update(parent)
{
}

QString UpdateDescription::description()
{
    return m_updatedDescription;
}

void UpdateDescription::exec(const TweetObject &tweet, const QString &newDescription)
{
    m_executedUser = tweet.user();
    emit stateChanged(Executed);

    qDebug() << "[Info] update_description: Update description to" << newDescription;

    try {
        m_updatedDescription = m_twitter.updateDescroption(newDescription).description();
        emit stateChanged(Updated);

        qDebug() << "[Info] update_description: Description updated."
                    "       New description:" << m_updatedDescription;

        if (m_settings.isPostUpdateDescriptionSuccessedMessage()) {
            recieveResult(m_settings.updateDescriptionSuccessedMessage()
                          .replace("%u", tweet.user().screen_name())
                          .replace("%d", m_updatedDescription), tweet.idStr());
        }
        
        if (m_settings.isWriteHistoryFile()) {
            UpdateHistory updateHistory;
            updateHistory.writeUpdateDescriptionHistory(tweet.user(), m_updatedDescription);
        }
    } catch (const std::runtime_error &e) {
        m_errorMessage = QString::fromStdString(e.what());
        emit error(UpdateFailed);

        qCritical() << "[Error] update_description: Update description failed."
                       "        Error message:" << m_errorMessage;

        if (m_settings.isPostUpdateDescriptionFailedMessage()) {
            recieveResult(m_settings.updateDescriptionFailedMessage()
                          .replace("%u", tweet.user().screen_name())
                          .replace("%d", newDescription)
                          .replace("%e", m_errorMessage));
        }
        return;
    }
}
