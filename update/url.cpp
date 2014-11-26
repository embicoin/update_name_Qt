#include "url.h"

UpdateUrl::UpdateUrl(QObject *parent) :
    Update(parent)
{
}

QString UpdateUrl::url()
{
    return m_updatedUrl;
}

void UpdateUrl::exec(const TweetObject &tweet, const QString &newUrl)
{
    m_executedUser = tweet.user();
    emit stateChanged(Executed);

    qDebug() << "[Info] update_url: Update url to" << newUrl;

    try {
        m_updatedUrl = m_twitter.updateUrl(newUrl).url().toString();
        emit stateChanged(Updated);

        qDebug() << "[Info] update_url: Url updated."
                    "       New url:" << m_updatedUrl;

        if(m_settings.isPostUpdateUrlSuccessedMessage()) {
            recieveResult(m_settings.updateUrlSuccessedMessage()
                          .replace("%u", tweet.user().screen_name())
                          .replace("%l", m_updatedUrl), tweet.idStr());
        }
    } catch(const std::runtime_error &e) {
        m_errorMessage = QString::fromStdString(e.what());
        emit error(UpdateFailed);

        qCritical() << "[Error] update_description: Update url failed."
                       "        Error message:" << m_errorMessage;

        if(m_settings.isPostUpdateUrlFailedMessage())
            recieveResult(m_settings.updateUrlFailedMessage()
                          .replace("%u", tweet.user().screen_name())
                          .replace("%l", newUrl)
                          .replace("%e", m_errorMessage), tweet.idStr());
        return;
    }
}
