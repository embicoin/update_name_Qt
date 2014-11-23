#include "image.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QTimer>
#include <QFile>
#include <QStandardPaths>
#include <QDir>
#include <QDateTime>

UpdateImage::updateImage(Update *parent)
    : Update(parent)
{
}

QUrl UpdateImage::imageUrl()
{
    return m_updatedImageUrl;
}

void UpdateImage::exec(const TweetObject &tweet, const QUrl &imageUrl)
{
    if(imageUrl.isEmpty() || !imageUrl.isValid())
        return;

    emit stateChanged(Executed);

    QNetworkAccessManager manager;
    QNetworkReply *reply;
    QTimer timer;
    QEventLoop loop;

    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));

    timer.setSingleShot(true);
    timer.start(15000);
    reply = manager.get(QNetworkReply(imageUrl));
    loop.exec();

    if(reply->error() == QNetworkReply::NoError && timer.isActive()) {
        try {
            m_twitter.updateProfileImage(reply->readAll());
        } catch(const std::runtime_error &e) {

        }
    }
}
