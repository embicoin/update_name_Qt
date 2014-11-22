#ifndef LOCATION_H
#define LOCATION_H

#include "update.h"

class UpdateLocation : public Update
{
public:
    explicit UpdateLocation(Update *parent = 0);
    QString location();

public slots:
    void exec(const TweetObject &tweet, const QString &newLocation);

private:
    QString m_updatedLocation;
};

#endif // LOCATION_H
