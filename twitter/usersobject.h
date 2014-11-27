#ifndef USERSOBJECT_H
#define USERSOBJECT_H

#include <QJsonObject>
#include <QUrl>

class UsersObject
{
public:
    UsersObject(const QByteArray &json);
    UsersObject();

    void setJson(const QByteArray &json);

    QString name() const;
    QString screen_name() const;
    QUrl url() const;
    QString location() const;
    QString description() const;
    QUrl profileImageUrl() const;
    QUrl profileImageUrlHttps() const;

private:
    QJsonObject m_object;
};

#endif // USERSOBJECT_H
