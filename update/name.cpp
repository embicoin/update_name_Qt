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

    const QString status_id = tweet.idStr();
    const QString user_screen_name = tweet.user().screen_name();

    emit stateChanged(Executed);

    /*nameは20文字まで*/
    if(newName.length() > 20) {
        emit stateChanged(Aborted);
        recieveResult(tr(".@%1 nameは20文字までです").arg(user_screen_name));
        return;
    }

    /*nameの更新*/
    try {
        m_twitter.updateName(newName);
    } catch(const std::runtime_error &e) {
        /*更新失敗時の処理*/
        m_errorMessage = QString::fromStdString(e.what());
        emit error(m_errorMessage);
        emit stateChanged(UpdateFailed);

        /*結果のツイート*/
        if(m_settings.isPostUpdateNameFailedMessage()) {
            recieveResult(m_settings.updateNameFailedMessage()
                          .replace("%u", user_screen_name)
                          .replace("%n", newName)
                          .replace("%e", m_errorMessage), status_id);
        }
        return;
    }

    /*更新後のnameの取得*/
    try {
        m_updatedName = m_twitter.verifyCredentials().name();
    } catch(...) {
        m_updatedName = newName;
    }

    emit stateChanged(UpdateSuccessed);
    emit updated(m_updatedName);

    if(m_settings.isPostUpdateNameSuccessedMessage()) {
        recieveResult(m_settings.updateNameSuccessedMessage()
                      .replace("%u", user_screen_name)
                      .replace("%n", m_updatedName), status_id);
    }
}
