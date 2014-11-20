#ifndef USERSOBJECT_H
#define USERSOBJECT_H

#include <QJsonObject>

class UsersObject
{
public:
    UsersObject(const QByteArray &json);

    QString name() const;
    QString screen_name() const;
    QString url() const;
    QString location() const;
    QString description() const;

private:
    QJsonObject object;
};

#endif // USERSOBJECT_H
