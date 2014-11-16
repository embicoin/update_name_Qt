#ifndef NAME_H
#define NAME_H

#include "update.h"

#include <QObject>

class UpdateName : public Update
{
    Q_OBJECT
public:
    explicit UpdateName(Update *parent = 0);
    QString name();

signals:

public slots:
    void exec(const TweetObject &tweet, const QString &new_name);

private:
    QString updated_name;
};

#endif // NAME_H
