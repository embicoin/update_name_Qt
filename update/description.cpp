#include "description.h"

UpdateDescription::UpdateDescription(Update *parent) :
    Update(parent)
{
}

QString UpdateDescription::description()
{
    return updated_description;
}

void UpdateDescription::exec(const TweetObject &tweet, const QString &new_description)
{
    if(tweet.text().isEmpty()) {
        return;
    }

    emit stateChanged(Executed);

    try {
        twitter.updateDescroption(new_description);
    } catch(const std::runtime_error &e) {
        error_message = QString::fromStdString(e.what());
        emit error(error_message);
        emit stateChanged(UpdateFailed);

        if(settings.isPostUpdateDescriptionFailedMessage()) {
            recieveResult(settings.updateDescriptionFailedMessage()
                          .replace("%u", tweet.user().screen_name())
                          .replace("%d", new_description)
                          .replace("%e", error_message));
        }
        return;
    }

    try {
        updated_description = twitter.verifyCredentials().description();
    } catch(...) {
        updated_description = new_description;
    }

    emit stateChanged(UpdateSuccessed);
    emit updated(updated_description);

    if(settings.isPostUpdateDescriptionSuccessedMessage()) {
        recieveResult(settings.updateDescriptionSuccessedMessage()
                      .replace("%u", tweet.user().screen_name())
                      .replace("%d", updated_description), tweet.idStr());
    }
}
