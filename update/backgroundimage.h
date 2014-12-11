#ifndef BACKGROUNDIMAGE_H
#define BACKGROUNDIMAGE_H

#include "update.h"

#include <QUrl>

class UpdateBackgroundImage : public Update
{
public:
    explicit UpdateBackgroundImage(QObject *parent = 0);
    QUrl backgroundImageUrl();

public slots:
    void exec(const TweetObject &tweet);

private:
    QUrl m_updatedBackgroundImageUrl;
};

#endif // BACKGROUNDIMAGE_H
