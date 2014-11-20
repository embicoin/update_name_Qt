#ifndef NAME_H
#define NAME_H

#include "update.h"

class UpdateName : public Update
{
public:
    explicit UpdateName(Update *parent = 0);
    QString name();

public slots:
    void exec(const TweetObject &tweet, const QString &new_name);

private:
    QString updated_name;
};

#endif // NAME_H
