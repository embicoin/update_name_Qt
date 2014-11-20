#include "location.h"

UpdateLocation::UpdateLocation(Update *parent) :
    Update(parent)
{
}

QString UpdateLocation::location()
{
    return updated_location;
}

void UpdateLocation::exec(const TweetObject &tweet, const QString &new_location)
{
    if(new_location.isEmpty()) {
        return;
    }

    emit stateChanged(Executed);

    try {
        twitter.updateLocation(new_location);
    } catch(const std::runtime_error &e) {
        error_message = e.what();
        emit error(error_message);
        emit stateChanged(UpdateFailed);

        if(settings.isPostUpdateLocationFailedMessage()) {
            recieveResult(settings.updateLocationFailedMessage()
                          .replace("%u", tweet.user().screen_name())
                          .replace("%l", new_location)
                          .replace("%e", error_message), tweet.idStr());
        }
        return;
    }

    try {
        updated_location = twitter.verifyCredentials().location();
    } catch(...) {
        updated_location = new_location;
    }

    emit stateChanged(UpdateSuccessed);
    emit updated(updated_location);

    if(settings.isPostUpdateLocationSuccessedMessage()) {
        recieveResult(settings.updateLocationSuccessedMessage()
                      .replace("%u", tweet.user().screen_name())
                      .replace("%l", updated_location), tweet.idStr());
    }
}
