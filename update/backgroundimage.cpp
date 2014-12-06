#include "backgroundimage.h"

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

UpdateBackgroundImage::UpdateBackgroundImage(QObject *parent)
    : Update(parent)
{
}

QUrl UpdateBackgroundImage::backgroundImageUrl()
{
    return m_updatedBackgroundImageUrl;
}


void UpdateBackgroundImage::exec(const TweetObject &tweet)
{
    QNetworkAccessManager manager;
    std::unique_ptr<QNetworkReply> reply;
    QTimer timer;
    QEventLoop loop;

    m_executedUser = tweet.user();
    emit stateChanged(Executed);

    qDebug() << "[Info] update_background_image: Update image to" << tweet.entities().media().mediaUrlHttps().toString();

    timer.setSingleShot(true);
    timer.start(15000);

    qDebug() << "[Info] update_background_image: Start download new image.";

    reply.reset(manager.get(QNetworkRequest(tweet.entities().media().mediaUrlHttps())));

    connect(reply.get(), SIGNAL(finished()), &loop, SLOT(quit()));
    connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));

    loop.exec();

    if (reply->error() == QNetworkReply::NoError && timer.isActive()) {

        qDebug() << "[Info] update_background_image: Download finished.";

        try {
            m_twitter.updateProfileBackground(reply->readAll());

            qDebug() << "[Info] update_background_image: Image updated.";
            qDebug() << "[Info] update_background_image: Getting current image.";

            try {
                m_updatedBackgroundImageUrl = m_twitter.verifyCredentials().profileBackgroundImageUrlHttps();
                qDebug() << "[Info] update_image: Current image is" << m_updatedBackgroundImageUrl;
            } catch (...) {
                m_updatedBackgroundImageUrl = tweet.entities().media().mediaUrlHttps();
                qWarning() << "[Warning] update_image: Getting current image failed.";
            }

            emit stateChanged(Updated);

            if (m_settings.isPostUpdateBackgroundSuccessedMessage())
                recieveResult(m_settings.updateBackgroundImageSuccessedMessage()
                              .replace("%u", tweet.user().screen_name())
                              .replace("%i", m_updatedBackgroundImageUrl.toString()), tweet.idStr());
            return;
        } catch (const std::runtime_error &e) {
            m_errorMessage = QString::fromStdString(e.what());
            qCritical() << "[Error] update_background_image: Update image failed."
                           "        Error message:" << m_errorMessage;
        }
    } else {
        m_errorMessage = reply->errorString();

        qCritical() << "[Error] update_background_image: Download failed."
                       "        Error message: " << m_errorMessage;
    }

    emit error(UpdateFailed);

    if (m_settings.isPostUpdateBackgroundFailedMessage())
        recieveResult(m_settings.updateBackgroundImageFailedMessage()
                      .replace("%u", tweet.user().screen_name())
                      .replace("%e", m_errorMessage));
}
