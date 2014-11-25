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

    emit executed(tweet.user());

    try {
        m_updatedUrl = m_twitter.updateUrl(newUrl).url().toString();
        emit updated(m_updatedUrl);
    } catch(const std::runtime_error &e) {
        m_errorMessage = QString::fromStdString(e.what());
        emit error(UpdateProfile, m_errorMessage);

        if(m_settings.isPostUpdateUrlFailedMessage())
            recieveResult(m_settings.updateUrlFailedMessage()
                          .replace("%u", tweet.user().screen_name())
                          .replace("%l", newUrl)
                          .replace("%e", m_errorMessage), tweet.idStr());
        return;
    }

    if(m_settings.isPostUpdateUrlSuccessedMessage()) {
        recieveResult(m_settings.updateUrlSuccessedMessage()
                      .replace("%u", tweet.user().screen_name())
                      .replace("%l", m_updatedUrl), tweet.idStr());
    }
}
