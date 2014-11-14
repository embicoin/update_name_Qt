#include "updatename.h"
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonObject>

UpdateName::UpdateName(QObject *parent) :
    QObject(parent)
{
    try {
        my_screen_name = twitter.getScreenName();
        emit stateChanged(GetScreenNameSuccessed);
    } catch(std::runtime_error &e) {
        error_message = QString::fromStdString(e.what());
        emit stateChanged(GetScreenNameFailed);
    }
}

QString UpdateName::lastErrorMessage()
{
    return error_message;
}

QString UpdateName::lastExecutedUpdateNameUser()
{
    return executed_update_name_user;
}

QString UpdateName::updatedName()
{
    return updated_name;
}

void UpdateName::postStartupMessage()
{
    try {
        twitter.statusUpdate(settings.startupMessage());
        emit stateChanged(PostStartupMessageSuccessed);
    } catch(std::runtime_error &e) {
        error_message = QString::fromStdString(e.what());
        emit stateChanged(PostStartupMessageFailed);
    }
}

void UpdateName::postClosedMessage()
{
    try {
        twitter.statusUpdate(settings.closedMessage());
        emit stateChanged(PostClosedMessageSuccessed);
    } catch(std::runtime_error &e) {
        error_message = QString::fromStdString(e.what());
        emit stateChanged(PostClosedMessageFailed);
    }
}

void UpdateName::exec(const QByteArray &twitter_status_object_json_data)
{
    QJsonObject status_object = QJsonDocument::fromJson(twitter_status_object_json_data).object();
    QJsonObject user_object = status_object.value("user").toObject();
    const QString text = status_object.value("text").toString();
    const QString status_id = status_object.value("id_str").toString();
    const QString user_screen_name = user_object.value("screen_name").toString();
    const QRegExp update_name_reg_exp1 = QRegExp("^.*@" + my_screen_name + "\\s+update_name\\s+.*");
    const QRegExp update_name_reg_exp2 = QRegExp("^\\s*.+\\s*\\(@" + my_screen_name + "\\).*$");
    QString new_name;


    if(text.isEmpty() || text.startsWith("RT")) {
        return;
    }

    if(update_name_reg_exp1.exactMatch(text)) {
        new_name = text;
        new_name.remove(QRegExp("^.*@" + my_screen_name + ("\\s+update_name\\s+")));
    } else if(update_name_reg_exp2.exactMatch(text)) {
        new_name = text;
        new_name.remove(QRegExp("\\s*\\(@" + my_screen_name + "\\).*$"));
    } else {
        return;
    }

    if(new_name.isEmpty()) {
        return;
    }

    executed_update_name_user = user_screen_name;
    emit stateChanged(Executed);

    if(new_name.length() > 20) {
        emit stateChanged(Aborted);
        try {
            twitter.statusUpdate(".@" + user_screen_name + tr("名前は20文字までですよ。"), status_id);
            emit stateChanged(ResultRecieved);
        } catch(std::runtime_error &e) {
            error_message = QString::fromStdString(e.what());
            emit stateChanged(RecieveResultFailed);
        }
        return;
    }

    try {
        twitter.updateName(new_name);
    } catch(std::runtime_error &e) {
        error_message = QString::fromStdString(e.what());
        emit stateChanged(UpdateNameFailed);

        if(settings.isPostUpdateNameFailedMessage()) {
            try {
                twitter.statusUpdate(settings.updateNameFailedMessage()
                                     .replace("%u", user_screen_name)
                                     .replace("%n", new_name)
                                     .replace("%e", QString::fromStdString(e.what())), status_id);
                emit stateChanged(ResultRecieved);
            } catch(std::runtime_error &e) {
                error_message = QString::fromStdString(e.what());
                emit stateChanged(RecieveResultFailed);
            }
        }

        return;
    }

    try {
        updated_name = twitter.getName();
    } catch(...) {
        updated_name = new_name;
    }

    emit stateChanged(NameUpdated);

    if(settings.isPostUpdateNameSuccessedMessage()) {
        try {
            twitter.statusUpdate(settings.updateNameSuccessedMessage()
                                 .replace("%u", user_screen_name)
                                 .replace("%n", new_name), status_id);
            emit stateChanged(ResultRecieved);
        } catch(std::runtime_error &e) {
            error_message = QString::fromStdString(e.what());
            emit stateChanged(RecieveResultFailed);
        }
    }
}
