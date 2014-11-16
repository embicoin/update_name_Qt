#ifndef NAME_H
#define NAME_H

#include "update.h"

#include <QObject>

class UpdateName : public Update
{
    Q_OBJECT
public:
    explicit UpdateName(Update *parent = 0);
    QString name();

signals:

protected:
    using Update::updateProfile;
    virtual void updateProfile(const TweetObject &tweet, const QString &update_profile_value);

public slots:

private:
    QString updated_name;
};

#endif // NAME_H
