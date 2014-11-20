#include "update.h"

Update::Update(QObject *parent) :
    QObject(parent)
{
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
