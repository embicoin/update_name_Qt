#include "update.h"

Update::Update(QObject *parent) :
    QObject(parent)
{
}

QString Update::errorString()
{
    return m_errorMessage;
}

void Update::recieveResult(const QString &message, const QString &inReplyToStatusId)
{
    try {
        m_twitter.statusUpdate(message, inReplyToStatusId);
        emit stateChanged(RecieveResultSuccessed);
    } catch(std::runtime_error &e) {
        m_errorMessage = QString::fromStdString(e.what());
        emit error(m_errorMessage);
        emit stateChanged(RecieveResultFailed);
    }
}
