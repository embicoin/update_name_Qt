#ifndef IMAGE_H
#define IMAGE_H

#include "update.h"

#include <QUrl>

class UpdateImage : public Update
{
public:
    explicit UpdateImage(Update *parent = 0);
    QUrl imageUrl();

public slots:
    void exec(const TweetObject &tweet);

private:
    QUrl m_updatedImageUrl;
};

#endif // IMAGE_H
