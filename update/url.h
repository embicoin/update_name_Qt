#ifndef UPDATEURL_H
#define UPDATEURL_H

#include "update.h"

class UpdateUrl : public Update
{
public:
    explicit UpdateUrl(Update *parent = 0);
    QString url();

public slots:
    void exec(const TweetObject &tweet, const QString &newUrl);

private:
    QString m_updatedUrl;
};

#endif // URL_H
