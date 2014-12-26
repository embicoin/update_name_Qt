#ifndef UPLOAD_H
#define UPLOAD_H

#include <QFile>
#include <QUrl>

#include "../restapi.h"
#include "../../object/error.h"

namespace TwitterAPI {
namespace Rest {
namespace Media {
class Upload : public RestApi
{
    Q_OBJECT
public:
    explicit Upload(QObject *parent = 0);
    explicit Upload(const TwitterAPI::OAuth &oauth, QObject *parent = 0);

signals:
    void finished();
    void successed(const QString &mediaId);
    void fileError(const QString &errorMessage);
    void apiError(const TwitterAPI::Object::Error &error);
    void networkError(const QString &errorMessage);

public slots:
    QString exec(const QString &filePath);
    QString exec(const QFile &mediaFile);

private:
    static const QUrl UPLOAD_URL;
};
}
}
}

#endif // UPLOAD_H
