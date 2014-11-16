#ifndef USERSOBJECT_H
#define USERSOBJECT_H

#include <QJsonObject>

class UsersObject
{
public:
    UsersObject(const QByteArray &json);

    QString name() const;
    QString screen_name() const;

private:
    QJsonObject object;
};

#endif // USERSOBJECT_H
