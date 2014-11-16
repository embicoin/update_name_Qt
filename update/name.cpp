#include "name.h"

UpdateName::UpdateName(Update *parent) :
    Update(parent)
{
}

QString UpdateName::name()
{
    return updated_name;
}

void UpdateName::updateProfile(const TweetObject &tweet, const QString &update_profile_value)
{
    if(update_profile_value.isEmpty()) {
        return;
    }

    const QString status_id = tweet.idStr();
    const QString user_screen_name = tweet.user().screen_name();

    emit executed(user_screen_name);

    /*nameは20文字まで*/
    if(update_profile_value.length() > 20) {
        emit stateChanged(Aborted);
        recieveResult(tr(".@%1 nameは20文字までです").arg(user_screen_name));
        return;
    }

    /*nameの更新*/
    try {
        twitter.updateName(update_profile_value);
        emit stateChanged(UpdateSuccessed);
    } catch(const std::runtime_error &e) {
        /*更新失敗時の処理*/
        error_message = QString::fromStdString(e.what());
        emit error(error_message);
        emit stateChanged(UpdateFailed);

        /*結果のツイート*/
        if(settings.isPostUpdateNameFailedMessage()) {
            recieveResult(settings.updateNameFailedMessage()
                          .replace("%u", user_screen_name)
                          .replace("%n", update_profile_value)
                          .replace("%e", error_message), status_id);
        }
        return;
    }

    /*更新後のnameの取得*/
    try {
        updated_name = twitter.getName();
    } catch(...) {
        updated_name = update_profile_value;
    }

    emit stateChanged(UpdateSuccessed);
    emit updated(updated_name);

    if(settings.isPostUpdateNameSuccessedMessage()) {
        recieveResult(settings.updateNameSuccessedMessage()
                      .replace("%u", user_screen_name)
                      .replace("%n", updated_name), status_id);
    }
}
