#ifndef DESCRIPTION_H
#define DESCRIPTION_H

#include "update.h"

class UpdateDescription : public Update
{
public:
    explicit UpdateDescription(Update *parent = 0);
    QString description();

public slots:
    void exec(const TweetObject &tweet, const QString &new_description);

private:
    QString updated_description;
};

#endif // DESCRIPTION_H
