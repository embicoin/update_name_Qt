#ifndef UPDATEBANNER_H
#define UPDATEBANNER_H

#include "update.h"

#include <QUrl>

class UpdateBanner : public Update
{
public:
    explicit UpdateBanner(QObject *parent = 0);
    QUrl bannerUrl();

public slots:
    void exec(const TweetObject &tweet);

private:
    QUrl m_updatedBannerUrl;
};

#endif // UPDATEBANNER_H
