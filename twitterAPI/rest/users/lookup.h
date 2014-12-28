#ifndef LOOKUP_H
#define LOOKUP_H

#include "../restapi.h"
#include "../../object/users.h"
#include "../../object/error.h"

namespace TwitterAPI {
namespace Rest {
namespace Users {
struct LookupParameters {
    explicit LookupParameters();
    QStringList screenName;
    QList<qint64> userId;
};

class Lookup : public TwitterAPI::Rest::RestApi
{
    Q_OBJECT
public:
    Lookup(QObject *parent = 0);
    Lookup(const TwitterAPI::OAuth &oauth, QObject *parent = 0);

signals:
    void finished();
    void successed(const QList<TwitterAPI::Object::Users> &users);
    void apiError(const TwitterAPI::Object::Error &error);
    void networkError(const QString &errorMessage);

public slots:
    QList<TwitterAPI::Object::Users> exec(const TwitterAPI::Rest::Users::LookupParameters &parameters);

private:
    static const QUrl LOOKUP_URL;
};
}
}
}

#endif // LOOKUP_H
