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
#include <QDebug>

UpdateImage::UpdateImage(Update *parent)
    : Update(parent)
{
}

QUrl UpdateImage::imageUrl()
{
    return m_updatedImageUrl;
}

void UpdateImage::exec(const TweetObject &tweet)
{
    emit executed(tweet.user());
    qDebug() << "udpate_image: " << "Update to " << tweet.entities().media().mediaUrlHttps().toString();

    QNetworkAccessManager manager;
    QNetworkReply *reply;
    QTimer timer;
    QEventLoop loop;

    timer.setSingleShot(true);
    timer.start(15000);
    reply = manager.get(QNetworkRequest(tweet.entities().media().mediaUrlHttps()));

    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));

    loop.exec();

    if(reply->error() == QNetworkReply::NoError && timer.isActive()) {
        try {
            m_twitter.updateProfileImage(reply->readAll());

            try {
                m_updatedImageUrl = m_twitter.verifyCredentials().profileImageUrlHttps();
            } catch(...) {
                m_updatedImageUrl = tweet.entities().media().mediaUrlHttps();
            }

            emit updated(m_updatedImageUrl.toString());

            if(m_settings.isPostUpdateImageSuccessedMessage())
                recieveResult(m_settings.updateImageSuccessedMessage()
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
