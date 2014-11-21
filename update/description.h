#ifndef DESCRIPTION_H
#define DESCRIPTION_H

#include "update.h"

class UpdateDescription : public Update
{
public:
    explicit UpdateDescription(Update *parent = 0);
    QString description();

public slots:
    void exec(const TweetObject &tweet, const QString &newDescription);

private:
    QString m_updatedDescription;
};

#endif // DESCRIPTION_H
