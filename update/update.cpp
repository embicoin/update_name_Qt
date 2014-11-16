#include "update.h"

Update::Update(QObject *parent) :
    QObject(parent)
{
}

void Update::updateProfile(const TweetObject, const QString)
{
}

void Update::exec(const TweetObject &tweet, const QString &update_profile_value)
{
    updateProfile(tweet, update_profile_value);
}

QString Update::errorString()
{
    return error_message;
}

void Update::recieveResult(const QString &message, const QString &in_reply_to_status_id)
{
    try {
        twitter.statusUpdate(message, in_reply_to_status_id);
        emit stateChanged(RecieveResultSuccessed);
    } catch(std::runtime_error &e) {
        error_message = QString::fromStdString(e.what());
        emit error(error_message);
        emit stateChanged(RecieveResultFailed);
    }
}
