#include "url.h"

UpdateUrl::UpdateUrl(Update *parent) :
    Update(parent)
{
}

QString UpdateUrl::url()
{
    return updated_url;
}

void UpdateUrl::exec(const TweetObject &tweet, const QString &new_url)
{
    if(new_url.isEmpty()) {
        return;
    }

    const QString status_id = tweet.idStr();
    const QString user_screen_name = tweet.user().screen_name();

    emit stateChanged(Executed);

    try {
        twitter.updateUrl(new_url);
    } catch(const std::runtime_error &e) {
        error_message = QString::fromStdString(e.what());
        emit error(error_message);
        emit stateChanged(UpdateFailed);

        if(settings.isPostUpdateUrlFailedMessage()) {
            recieveResult(settings.updateUrlFailedMessage()
                          .replace("%u", user_screen_name)
                          .replace("%l", new_url)
                          .replace("%e", error_message), status_id);
        }
        return;
    }

    try {
        updated_url = twitter.verifyCredentials().url();
    } catch(...) {
        updated_url = new_url;
    }

    emit stateChanged(UpdateSuccessed);
    emit updated(updated_url);

    if(settings.isPostUpdateUrlSuccessedMessage()) {
        recieveResult(settings.updateUrlSuccessedMessage()
                      .replace("%u", user_screen_name)
                      .replace("%l", updated_url), status_id);
    }
}
