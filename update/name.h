#ifndef NAME_H
#define NAME_H

#include "update.h"

class UpdateName : public Update
{
public:
    explicit UpdateName(QObject *parent = 0);
    QString name();

public slots:
    void exec(const TweetObject &tweet, const QString &newName);

private:
    QString m_updatedName;
};

#endif // NAME_H
