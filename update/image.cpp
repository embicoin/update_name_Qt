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
#include <memory>

UpdateImage::UpdateImage(QObject *parent)
    : Update(parent)
{
}

QUrl UpdateImage::imageUrl()
{
    return m_updatedImageUrl;
}

void UpdateImage::exec(const TweetObject &tweet)
{
    QNetworkAccessManager manager;
    std::unique_ptr<QNetworkReply> reply;
    QTimer timer;
    QEventLoop loop;

    m_executedUser = tweet.user();
    emit stateChanged(Executed);

    qDebug() << "[Info] update_image: Update image to " << tweet.entities().media().mediaUrlHttps().toString();

    timer.setSingleShot(true);
    timer.start(15000);

    qDebug() << "[Info] update_image: Start download new image.";

    reply.reset(manager.get(QNetworkRequest(tweet.entities().media().mediaUrlHttps())));

    connect(reply.get(), SIGNAL(finished()), &loop, SLOT(quit()));
    connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));

    loop.exec();

    if (reply->error() == QNetworkReply::NoError && timer.isActive()) {

        qDebug() << "[Info] update_image: Download finished.";

        try {
            m_twitter.updateProfileImage(reply->readAll());

            qDebug() << "[Info] update_image: Image updated.";
            qDebug() << "[Info] update_image: Getting current image.";

            try {
                m_updatedImageUrl = m_twitter.verifyCredentials().profileImageUrlHttps();
                qDebug() << "[Info] update_image: Current image is" << m_updatedImageUrl;
            } catch(...) {
                m_updatedImageUrl = tweet.entities().media().mediaUrlHttps();
                qWarning() << "[Warning] update_image: Getting current image failed.";
            }

            emit stateChanged(Updated);

            if (m_settings.isPostUpdateImageSuccessedMessage())
                recieveResult(m_settings.updateImageSuccessedMessage()
                              .replace("%u", tweet.user().screen_name())
                              .replace("%i", m_updatedImageUrl.toString()), tweet.idStr());
            return;
        } catch (const std::runtime_error &e) {
            m_errorMessage = QString::fromStdString(e.what());

            qCritical() << "[Error] update_image: Update image failed."
                           "        Error message:" << m_errorMessage;
        }
    } else {
        m_errorMessage = reply->errorString();

        qCritical() << "[Error] update_image: Download failed."
                       "        Error message: " << m_errorMessage;
    }

    emit error(UpdateFailed);

    if (m_settings.isPostUpdateImageFailedMessage())
        recieveResult(m_settings.updateImageFailedMessage()
                      .replace("%u", tweet.user().screen_name())
                      .replace("%e", m_errorMessage), tweet.idStr());
}
