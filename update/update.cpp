#include "update.h"
#include "../update_nane_qt_global.h"

Update::Update(QObject *parent) :
    QObject(parent)
{
    qRegisterMetaType<Update::State>("Update::State");
    qRegisterMetaType<Update::ErrorState>("Update::ErrorState");
}

QString Update::errorString()
{
    return m_errorMessage;
}

UsersObject Update::executedUser()
{
    return m_executedUser;
}

void Update::recieveResult(const QString &message, const QString &inReplyToStatusId)
{
    bool reTry = false;
    QString endString;
    do {
        try {
            m_twitter.statusUpdate(message + endString, inReplyToStatusId);
            emit stateChanged(ResultRecieved);

            qDebug() << "[Info] update: Result recieved.";

            reTry = false;

        } catch(std::runtime_error &e) {
            m_errorMessage = QString::fromStdString(e.what());
            emit error(ResultRecieveFailed);

            qCritical() << "[Error] update: Result recieve failed.\n"
                           "        Error message:" << m_errorMessage;

            if (m_settings.isRetryTweetOnStatusIsADuplicate() && strcmp(e.what() ,"Status is a duplicate.") == 0
                    && message.length() + endString.length() <= 140) {
                reTry = true;
                endString.append(UpdateNameQt::randomEndString());
            } else {
                reTry = false;
            }
        }
    } while (m_settings.isRetryTweetOnStatusIsADuplicate() && reTry);
}
