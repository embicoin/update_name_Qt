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

UpdateImage::UpdateImage(Update *parent)
    : Update(parent)
{
}

QUrl UpdateImage::imageUrl()
{
    return m_updatedImageUrl;
}

void UpdateImage::exec(const TweetObject &tweet, const QUrl &imageUrl)
{
    if(imageUrl.isEmpty() || !imageUrl.isValid()) {
        return;
    }

    emit executed(tweet.user());

    QNetworkAccessManager manager;
    QNetworkReply *reply;
    QTimer timer;
    QEventLoop loop;

    timer.setSingleShot(true);
    timer.start(15000);
    reply = manager.get(QNetworkRequest(imageUrl));

    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));

    loop.exec();

    if(reply->error() == QNetworkReply::NoError && timer.isActive()) {
        try {
            m_updatedImageUrl = m_twitter.updateProfileImage(reply->readAll()).profileImageUrlHttps();

            emit updated(m_updatedImageUrl.toString());

            if(m_settings.isPostUpdateImageSuccessedMessage())
                recieveResult(m_settings.updateNameSuccessedMessage()
                              .replace("%u", tweet.user().screen_name())
                              .replace("%i", m_updatedImageUrl.toString()), tweet.idStr());
            return;
        } catch(const std::runtime_error &e) {
            m_errorMessage = QString::fromStdString(e.what());
        }
    } else {
        m_errorMessage = reply->errorString();
    }

    emit error(UpdateProfile, m_errorMessage);

    if(m_settings.isPostUpdateImageFailedMessage()) {
        recieveResult(m_settings.updateImageFailedMessage()
                      .replace("%u", tweet.user().screen_name())
                      .replace("%e", m_errorMessage), tweet.idStr());
    }
}
