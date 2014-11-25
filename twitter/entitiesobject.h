#ifndef ENTITIESOBJECT_H
#define ENTITIESOBJECT_H

#include <QJsonObject>
#include <QUrl>

class EntitiesObject
{
public:
    class MediaObject
    {
    public :
        MediaObject(const QByteArray &json);

        QString displayUrl() const;
        QUrl expandedUrl() const;
        QString idStr() const;
        QUrl mediaUrl() const;
        QUrl mediaUrlHttps() const;
    private:
        QJsonObject m_object;
    };
    EntitiesObject(const QByteArray &json);

    MediaObject media() const;
private:
    QJsonObject m_object;
};

#endif // ENTITIESOBJECT_H
