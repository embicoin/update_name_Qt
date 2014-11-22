#include "url.h"

UpdateUrl::UpdateUrl(Update *parent) :
    Update(parent)
{
}

QString UpdateUrl::url()
{
    return m_updatedUrl;
}

void UpdateUrl::exec(const TweetObject &tweet, const QString &newUrl)
{
    if(newUrl.isEmpty()) {
        return;
    }

    const QString status_id = tweet.idStr();
    const QString user_screen_name = tweet.user().screen_name();

    emit stateChanged(Executed);

    try {
        m_twitter.updateUrl(newUrl);
    } catch(const std::runtime_error &e) {
        m_errorMessage = QString::fromStdString(e.what());
        emit error(m_errorMessage);
        emit stateChanged(UpdateFailed);

        if(m_settings.isPostUpdateUrlFailedMessage()) {
            recieveResult(m_settings.updateUrlFailedMessage()
                          .replace("%u", user_screen_name)
                          .replace("%l", newUrl)
                          .replace("%e", m_errorMessage), status_id);
        }
        return;
    }

    try {
        m_updatedUrl = m_twitter.verifyCredentials().url();
    } catch(...) {
        m_updatedUrl = newUrl;
    }

    emit stateChanged(UpdateSuccessed);
    emit updated(m_updatedUrl);

    if(m_settings.isPostUpdateUrlSuccessedMessage()) {
        recieveResult(m_settings.updateUrlSuccessedMessage()
                      .replace("%u", user_screen_name)
                      .replace("%l", m_updatedUrl), status_id);
    }
}
