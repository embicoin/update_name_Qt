#include "name.h"

#include <QDebug>

UpdateName::UpdateName(QObject *parent) :
    Update(parent)
{
}

QString UpdateName::name()
{
    return m_updatedName;
}

void UpdateName::exec(const TweetObject &tweet, QString newName)
{
    m_executedUser = tweet.user();
    emit stateChanged(Executed);

    if(!m_settings.updateNameFormat().isEmpty()) {
        newName = m_settings.updateNameFormat().replace("%n", newName);
    }

    qDebug() << "[Info] update_name: Update name to" << newName;

    try {
        m_updatedName = m_twitter.updateName(newName).name();
        emit stateChanged(Updated);

        qDebug() << "[info] update_name: Name updated."
                    "       New name:"<< m_updatedName;

        if(m_settings.isPostUpdateNameSuccessedMessage()) {
            recieveResult(m_settings.updateNameSuccessedMessage()
                          .replace("%u", tweet.user().screen_name())
                          .replace("%n", m_updatedName), tweet.idStr());
        }
    } catch(const std::runtime_error &e) {
        m_errorMessage = QString::fromStdString(e.what());
        emit error(UpdateFailed);

        qCritical() << "[Error] update_name: Update name failed."
                       "        Error message:"<< m_errorMessage;

        if(m_settings.isPostUpdateNameFailedMessage())
            recieveResult(m_settings.updateNameFailedMessage()
                          .replace("%u", tweet.user().screen_name())
                          .replace("%n", newName)
                          .replace("%e", m_errorMessage), tweet.idStr());
    }
}
