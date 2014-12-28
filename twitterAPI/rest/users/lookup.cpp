#include "lookup.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>

using namespace TwitterAPI::Rest::Users;

LookupParameters::LookupParameters()
{
    qRegisterMetaType<TwitterAPI::Rest::Users::LookupParameters>("TwitterAPI::Rest::Users::LookupParameters");
}

const QUrl Lookup::LOOKUP_URL("https://api.twitter.com/1.1/users/lookup.json");

Lookup::Lookup(QObject *parent)
    : RestApi(parent)
{
}

Lookup::Lookup(const TwitterAPI::OAuth &oauth, QObject *parent)
    : RestApi(oauth, parent)
{
}

QList<TwitterAPI::Object::Users> Lookup::exec(const LookupParameters &parameters)
{
    QVariantMap params;
    QJsonArray array;
    if (!parameters.screenName.isEmpty()) {
        QStringList screenNames;
        for (int i = 0; i < 100 && i < parameters.screenName.size(); i++)
            screenNames << parameters.screenName.at(i);
        params["screen_name"] = screenNames.join(",");
    }
    if (!parameters.userId.isEmpty()) {
        QStringList userIds;
        for (int i = 0; i < 100 && i < parameters.userId.size(); i++)
            userIds << QString::number(parameters.userId.at(i));
        params["user_id"] = userIds.join(",");
    }
    try {
        array = QJsonDocument::fromJson(requestApi(QNetworkAccessManager::GetOperation, LOOKUP_URL, params)).array();
        QList<TwitterAPI::Object::Users> users;
        for (auto i = array.constBegin(); i != array.constEnd(); i++) {
            QJsonDocument doc;
            doc.setObject((*i).toObject());
            users << TwitterAPI::Object::Users(doc.toJson());
        }
        emit successed(users);
        return users;
    } catch (const std::runtime_error &e) {
        emit networkError(QString::fromStdString(e.what()));
    } catch (const TwitterAPI::Object::Error &e) {
        emit apiError(e);
    }

    return QList<TwitterAPI::Object::Users>();
}
