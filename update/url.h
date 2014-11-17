#ifndef UPDATEURL_H
#define UPDATEURL_H

#include "update.h"

class UpdateUrl : public Update
{
public:
    UpdateUrl(Update *parent = 0);
    QString url();

public slots:
    void exec(const TweetObject &tweet, const QString &new_url);

private:
    QString updated_url;
};

#endif // URL_H
