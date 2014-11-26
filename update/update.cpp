#include "update.h"

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
    try {
        m_twitter.statusUpdate(message, inReplyToStatusId);
        emit stateChanged(ResultRecieved);

        qDebug() << "[Info] update: Result recieved.";

    } catch(std::runtime_error &e) {
        m_errorMessage = QString::fromStdString(e.what());
        emit error(ResultRecieveFailed);

        qCritical() << "[Error] update: Result recieve failed."
                       "        Error message:" << m_errorMessage;

    }
}
