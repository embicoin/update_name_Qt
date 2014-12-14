#include "banner.h"
#include "../updatehistory.h"

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

UpdateBanner::UpdateBanner(QObject *parent)
    : Update(parent)
{
}

QUrl UpdateBanner::bannerUrl()
{
    return m_updatedBannerUrl;
}

void UpdateBanner::exec(const TweetObject &tweet)
{
    QNetworkAccessManager manager;
    std::unique_ptr<QNetworkReply> reply;
    QTimer timer;
    QEventLoop loop;

    m_executedUser = tweet.user();
    emit stateChanged(Executed);

    qDebug() << "[Info] update_banner: Update banner to" << tweet.entities().media().mediaUrlHttps();

    timer.setSingleShot(true);
    timer.start(15000);

    qDebug() << "[Info] update_banner: Start download new banner.";

    reply.reset(manager.get(QNetworkRequest(tweet.entities().media().mediaUrlHttps())));

    connect(reply.get(), SIGNAL(finished), &loop, SLOT(quit()));
    connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));

    loop.exec();

    if (reply->error() == QNetworkReply::NoError && timer.isActive()) {

        qDebug() << "[Info] udpate_banner: Download finished.";

        try {
            m_twitter.updateProfileBackground(reply->readAll());

            qDebug() << "[Info] update_banner: Banner updated.";
            qDebug() << "[Info] update_banner: Getting current banner.";

            try {
                m_updatedBannerUrl = m_twitter.usersLookup(QString::null, m_settings.userId()).profileBannerUrl();
                qDebug() << "[Info] update_banner: Current banner is" << m_updatedBannerUrl;
            } catch (...) {
                m_updatedBannerUrl = tweet.entities().media().mediaUrlHttps();
                qWarning() << "[Warning] update_banner: Getting current banner failed.";
            }

            emit stateChanged(Updated);

            if (m_settings.isPostUpdateBannerSuccessedMessage())
                recieveResult(m_settings.updateBannerSuccessedMessage()
                              .replace("%u", tweet.user().screen_name())
                              .replace("%i", m_updatedBannerUrl.toString()));

            if (m_settings.isWriteHistoryFile()) {
                UpdateHistory history;
                history.writeUpdateBannerHistory(tweet.user());
            }

            return;
        } catch (const std::runtime_error &e) {
            m_errorMessage = QString::fromStdString(e.what());
            qCritical() << "[Error] update_banner: Update banner failed."
                           "        Error message:" << m_errorMessage;
        }
    } else {
        m_errorMessage = reply->errorString();
        qCritical() << "[Error] update_banner: Download failed."
                       "        Error message: " << m_errorMessage;
    }

    emit error(UpdateFailed);

    if (m_settings.isPostUpdateBannerFailedMesage())
        recieveResult(m_settings.updateBannerFailedMessage()
                      .replace("%u", tweet.user().screen_name())
                      .replace("%e", m_errorMessage));
}
