#include "name.h"

UpdateName::UpdateName(Update *parent) :
    Update(parent)
{
}

QString UpdateName::name()
{
    return m_updatedName;
}

void UpdateName::exec(const TweetObject &tweet, const QString &newName)
{
    if(newName.isEmpty()) {
        return;
    }

    emit executed(tweet.user());

    /*nameの更新*/
    try {
        m_updatedName = m_twitter.updateName(newName).name();
        emit updated(m_updatedName);
    } catch(const std::runtime_error &e) {
        /*更新失敗時の処理*/
        m_errorMessage = QString::fromStdString(e.what());
        emit error(UpdateProfile, m_errorMessage);

        /*結果のツイート*/
        if(m_settings.isPostUpdateNameFailedMessage())
            recieveResult(m_settings.updateNameFailedMessage()
                          .replace("%u", tweet.user().screen_name())
                          .replace("%n", newName)
                          .replace("%e", m_errorMessage), tweet.idStr());
        return;
    }

    if(m_settings.isPostUpdateNameSuccessedMessage()) {
        recieveResult(m_settings.updateNameSuccessedMessage()
                      .replace("%u", tweet.user().screen_name())
                      .replace("%n", m_updatedName), tweet.idStr());
    }
}
