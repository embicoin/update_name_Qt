#include "updateprofile.h"
#include "twitter/tweetobject.h"
#include "update/name.h"
#include "update/url.h"
#include "update/location.h"
#include "update/description.h"
#include "update/image.h"
#include "update/backgroundimage.h"
#include "update/banner.h"

#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonObject>
#include <QThread>
#include <QDebug>

QString UpdateProfile::m_myscreenname;
const QStringList UpdateProfile::updateCommands = QStringList() << tr("name")
                                                                << tr("url")
                                                                << tr("location")
                                                                << tr("description")
                                                                << tr("image")
                                                                << tr("background_image")
                                                                << tr("banner");

UpdateProfile::UpdateProfile(QObject *parent) :
    QObject(parent)
{
    qRegisterMetaType<UpdateProfile::ProfileType>("UpdateProfile::ProfileType");
    qRegisterMetaType<UpdateProfile::State>("UpdateProfile::State");
    qRegisterMetaType<UpdateProfile::ErrorState>("UpdateProfile::ErrorState");

    QMetaObject::invokeMethod(this, "getScreenName", Qt::QueuedConnection);
}

QString UpdateProfile::screenName()
{
    return m_myscreenname;
}

QString UpdateProfile::errorString()
{
    return m_errormessage;
}

QString UpdateProfile::profileTypeString(const UpdateProfile::ProfileType &type)
{
    switch(type) {
    case Name:
        return tr("name");
    case Url:
        return tr("url");
    case Location:
        return tr("location");
    case Description:
        return tr("description");
    case Image:
        return tr("image");
    case BackgroundImage:
        return tr("background_image");
    case Banner:
        return tr("banner");
    }
    return QString::null;
}

void UpdateProfile::postStartupMessage()
{
    try {
        m_twitter.statusUpdate(m_settings.startupMessage());
    } catch(...) {
        throw;
    }
}

void UpdateProfile::postClosedMessage()
{
    try {
        m_twitter.statusUpdate(m_settings.closedMessage());
    } catch(...) {
        throw;
    }
}

void UpdateProfile::exec(const QByteArray &twitter_status_object_json_data)
{
    const TweetObject tweet(twitter_status_object_json_data);
    const QRegExp updateNameRegExp("^\\s*.+\\s*\\(@" + m_myscreenname + "\\).*$");
    QString command;

    if (m_myscreenname.isEmpty()) {
        if (!getScreenName()) {
            if (m_settings.screenName().isEmpty())
                return;
            else
                m_myscreenname = m_settings.screenName();
        }
    }

    if (!tweet.text().isEmpty() && !tweet.text().startsWith("RT")) {
        if (updateNameRegExp.exactMatch(tweet.text())) {
            if (m_settings.isEnabledUpdateName()) {
                qDebug() << "[Info] UpdateProfile: update_name executed.";
                m_executeduser = tweet.user().screen_name();
                m_profilevalue = tweet.text();
                m_profilevalue.remove(QRegExp("\\s*\\(@" + m_myscreenname + "\\).*$"));

                UpdateName *updateName = new UpdateName;
                QThread *updateNameThread = new QThread;

                connect(updateName, &UpdateName::stateChanged, [&](const Update::State &state) {
                    switch (state) {
                    case Update::Executed:
                        emit executed(Name, updateName->executedUser());
                        break;
                    case Update::Updated:
                        emit updated(Name, updateName->name());
                        break;
                    case Update::ResultRecieved:
                        emit resultRecieved(Name);
                        break;
                    }
                });
                connect(updateName, &UpdateName::error, [&](const Update::ErrorState &state) {
                    switch (state) {
                    case Update::UpdateFailed:
                        emit updateError(Name, state, updateName->errorString());
                        break;
                    case Update::ResultRecieveFailed:
                        emit updateError(Name, state, updateName->errorString());
                        break;
                    }
                });
                connect(updateName, SIGNAL(finished()), updateNameThread, SLOT(quit()));
                connect(updateNameThread, SIGNAL(finished()), updateName, SLOT(deleteLater()));
                connect(updateNameThread, SIGNAL(finished()), updateNameThread, SLOT(deleteLater()));

                updateName->moveToThread(updateNameThread);
                updateNameThread->start();
                updateName->exec(tweet, m_profilevalue);
            } else {
                qWarning() << "[Warning] UpdateProfile: update_name is disabled.";
            }
        } else {
            bool isUpdateCommand = false;
            foreach (command, updateCommands) {
                const QRegExp updateRegExp("^.*@" + m_myscreenname + "\\s+update_" + command + "\\s+.*");
                if (updateRegExp.exactMatch(tweet.text())) {
                    m_executeduser = tweet.user().screen_name();
                    m_profilevalue = tweet.text();
                    m_profilevalue.remove(QRegExp("^.*@" + m_myscreenname + "\\s+update_" + command + "\\s+"));
                    isUpdateCommand = true;
                    break;
                }
            }
            if (isUpdateCommand) {
                if (command == "name") {
                    if (m_settings.isEnabledUpdateName()) {
                        qDebug() << "[Info] UpdateProfile: update_name executed.";

                        UpdateName *updateName = new UpdateName;
                        QThread *updateNameThread = new QThread;

                        connect(updateName, &UpdateName::stateChanged, [&](const Update::State &state) {
                            switch (state) {
                            case Update::Executed:
                                emit executed(Name, updateName->executedUser());
                                break;
                            case Update::Updated:
                                emit updated(Name, updateName->name());
                                break;
                            case Update::ResultRecieved:
                                emit resultRecieved(Name);
                                break;
                            }
                        });
                        connect(updateName, &UpdateName::error, [&](const Update::ErrorState &state) {
                            switch(state) {
                            case Update::UpdateFailed:
                                emit updateError(Name, state, updateName->errorString());
                                break;
                            case Update::ResultRecieveFailed:
                                emit updateError(Name, state, updateName->errorString());
                                break;
                            }
                        });
                        connect(updateName, SIGNAL(finished()), updateNameThread, SLOT(quit()));
                        connect(updateNameThread, SIGNAL(finished()), updateName, SLOT(deleteLater()));
                        connect(updateNameThread, SIGNAL(finished()), updateNameThread, SLOT(deleteLater()));

                        updateName->moveToThread(updateNameThread);
                        updateNameThread->start();
                        updateName->exec(tweet, m_profilevalue);
                    } else {
                        qWarning() << "[Warning] UpdateProfile: update_name is disabled.";
                    }
                } else if (command == "url") {
                    if (m_settings.isEnabledUpdateUrl()) {
                        qDebug() << "[Info] UpdateProfile: update_url executed.";

                        UpdateUrl *updateUrl = new UpdateUrl;
                        QThread *updateUrlThread = new QThread;

                        connect(updateUrl, &UpdateUrl::stateChanged, [&](const Update::State &state) {
                            switch (state) {
                            case Update::Executed:
                                emit executed(Url, updateUrl->executedUser());
                                break;
                            case Update::Updated:
                                emit updated(Url, updateUrl->url());
                                break;
                            case Update::ResultRecieved:
                                emit resultRecieved(Url);
                                break;
                            }
                        });
                        connect(updateUrl, &UpdateUrl::error, [&](const Update::ErrorState &state) {
                            switch(state) {
                            case Update::UpdateFailed:
                                emit updateError(Url, state, updateUrl->errorString());
                                break;
                            case Update::ResultRecieveFailed:
                                emit updateError(Url, state, updateUrl->errorString());
                                break;
                            }
                        });
                        connect(updateUrl, SIGNAL(finished()), updateUrlThread, SLOT(quit()));
                        connect(updateUrlThread, SIGNAL(finished()), updateUrl, SLOT(deleteLater()));
                        connect(updateUrlThread, SIGNAL(finished()), updateUrlThread, SLOT(deleteLater()));

                        updateUrl->moveToThread(updateUrlThread);
                        updateUrlThread->start();
                        updateUrl->exec(tweet, m_profilevalue);
                    } else {
                        qWarning() << "[Warning] UpdateProfile: update_url is disabled.";
                    }
                } else if (command == "location") {
                    if(m_settings.isEnabledUpdateLocation()) {
                        qDebug() << "[Info] UpdateProfile: update_location executed.";

                        UpdateLocation *updateLocation = new UpdateLocation;
                        QThread *updateLocationThread = new QThread;

                        connect(updateLocation, &UpdateLocation::stateChanged, [&](const Update::State &state) {
                            switch (state) {
                            case Update::Executed:
                                emit executed(Location, updateLocation->executedUser());
                                break;
                            case Update::Updated:
                                emit updated(Location, updateLocation->location());
                                break;
                            case Update::ResultRecieved:
                                emit resultRecieved(Location);
                                break;
                            }
                        });
                        connect(updateLocation, &UpdateLocation::error, [&](const Update::ErrorState &state) {
                            switch(state) {
                            case Update::UpdateFailed:
                                emit updateError(Location, state, updateLocation->errorString());
                                break;
                            case Update::ResultRecieveFailed:
                                emit updateError(Location, state, updateLocation->errorString());
                                break;
                            }
                        });
                        connect(updateLocation, SIGNAL(finished()), updateLocationThread, SLOT(quit()));
                        connect(updateLocationThread, SIGNAL(finished()), updateLocation, SLOT(deleteLater()));
                        connect(updateLocationThread, SIGNAL(finished()), updateLocationThread, SLOT(deleteLater()));

                        updateLocation->moveToThread(updateLocationThread);
                        updateLocationThread->start();
                        updateLocation->exec(tweet, m_profilevalue);
                    } else {
                        qWarning() << "[Warning] UpdateProfile: udpate_location is disabled.";
                    }
                } else if (command == "description") {
                    if(m_settings.isEnabledUpdateDescription()) {
                        qDebug() << "[Info] UpdateProfile: update_description executed.";

                        UpdateDescription *updateDescription = new UpdateDescription;
                        QThread *updateDescriptionThread = new QThread;

                        connect(updateDescription, &UpdateDescription::stateChanged, [&](const Update::State &state) {
                            switch (state) {
                            case Update::Executed:
                                emit executed(Description, updateDescription->executedUser());
                                break;
                            case Update::Updated:
                                emit updated(Description, updateDescription->description());
                                break;
                            case Update::ResultRecieved:
                                emit resultRecieved(Description);
                                break;
                            }
                        });
                        connect(updateDescription, &UpdateDescription::error, [&](const Update::ErrorState &state) {
                            switch(state) {
                            case Update::UpdateFailed:
                                emit updateError(Description, state, updateDescription->errorString());
                                break;
                            case Update::ResultRecieveFailed:
                                emit updateError(Description, state, updateDescription->errorString());
                                break;
                            }
                        });
                        connect(updateDescription, SIGNAL(finished()), updateDescriptionThread, SLOT(quit()));
                        connect(updateDescriptionThread, SIGNAL(finished()), updateDescription, SLOT(deleteLater()));
                        connect(updateDescriptionThread, SIGNAL(finished()), updateDescriptionThread, SLOT(deleteLater()));

                        updateDescription->moveToThread(updateDescriptionThread);
                        updateDescriptionThread->start();
                        updateDescription->exec(tweet, m_profilevalue);
                    } else {
                        qWarning() << "[Warning] UpdateProfile: udpate_description is disabled.";
                    }
                } else if (command == "image") {
                    if (m_settings.isEnabledUpdateImage()) {
                        qDebug() << "[Info] UpdateProfile: update_image executed.";

                        UpdateImage *updateImage = new UpdateImage;
                        QThread *updateImageThread = new QThread;

                        connect(updateImage, &UpdateImage::stateChanged, [&](const Update::State &state) {
                            switch (state) {
                            case Update::Executed:
                                emit executed(Image, updateImage->executedUser());
                                break;
                            case Update::Updated:
                                emit updated(Image, updateImage->imageUrl().toString());
                                break;
                            case Update::ResultRecieved:
                                emit resultRecieved(Image);
                                break;
                            }
                        });
                        connect(updateImage, &UpdateImage::error, [&](const Update::ErrorState &state) {
                            switch(state) {
                            case Update::UpdateFailed:
                                emit updateError(Image, state, updateImage->errorString());
                                break;
                            case Update::ResultRecieveFailed:
                                emit updateError(Image, state, updateImage->errorString());
                                break;
                            }
                        });
                        connect(updateImage, SIGNAL(finished()), updateImageThread, SLOT(quit()));
                        connect(updateImageThread, SIGNAL(finished()), updateImage, SLOT(deleteLater()));
                        connect(updateImageThread, SIGNAL(finished()), updateImageThread, SLOT(deleteLater()));

                        updateImage->moveToThread(updateImageThread);
                        updateImageThread->start();
                        updateImage->exec(tweet);
                    } else {
                        qWarning() << "[Warning] UpdateProfile: update_image is disabled.";
                    }
                } else if (command == "background_image") {
                    if (m_settings.isEnabledUpdateBackgroundImage()) {
                        qDebug() << "[Info] UpdateProfile: update_background_image executed.";

                        UpdateBackgroundImage *updateBackgroundImage = new UpdateBackgroundImage;
                        QThread *updateBackgroundImageThread = new QThread;

                        connect(updateBackgroundImage, &UpdateBackgroundImage::stateChanged, [&](const Update::State &state) {
                            switch (state) {
                            case Update::Executed:
                                emit executed(BackgroundImage, updateBackgroundImage->executedUser());
                                break;
                            case Update::Updated:
                                emit updated(BackgroundImage, updateBackgroundImage->backgroundImageUrl().toString());
                                break;
                            case Update::ResultRecieved:
                                emit resultRecieved(BackgroundImage);
                                break;
                            }
                        });
                        connect(updateBackgroundImage, &UpdateBackgroundImage::error, [&](const Update::ErrorState &state) {
                            switch (state) {
                            case Update::UpdateFailed:
                                emit updateError(BackgroundImage, state, updateBackgroundImage->errorString());
                                break;
                            case Update::ResultRecieveFailed:
                                emit updateError(BackgroundImage, state, updateBackgroundImage->errorString());
                                break;
                            }
                        });
                        connect(updateBackgroundImage, SIGNAL(finished()), updateBackgroundImageThread, SLOT(quit()));
                        connect(updateBackgroundImageThread, SIGNAL(finished()), updateBackgroundImage, SLOT(deleteLater()));
                        connect(updateBackgroundImageThread, SIGNAL(finished()), updateBackgroundImageThread, SLOT(deleteLater()));

                        updateBackgroundImage->moveToThread(updateBackgroundImageThread);
                        updateBackgroundImageThread->start();
                        updateBackgroundImage->exec(tweet);
                    } else {
                        qWarning() << "[Warning] UpdateProfile: update_background_image is disabled.";
                    }
                } else if (command == "banner") {
                    if (m_settings.isEnabledUpdateBanner()) {
                        qDebug() << "[Info] UpdateProfile: update_banner executed.";

                        UpdateBanner *updateBanner = new UpdateBanner;
                        QThread *updateBannerThread = new QThread;

                        connect(updateBanner, &UpdateBanner::stateChanged, [&](const Update::State &state) {
                            switch (state) {
                            case Update::Executed:
                                emit executed(Banner, updateBanner->executedUser());
                                break;
                            case Update::Updated:
                                emit updated(Banner, updateBanner->bannerUrl().toString());
                                break;
                            case Update::ResultRecieved:
                                emit resultRecieved(Banner);
                                break;
                            }
                        });
                        connect(updateBanner, &UpdateBanner::error, [&](const Update::ErrorState &state) {
                            emit updateError(Banner, state, updateBanner->errorString());
                        });
                        connect(updateBanner, SIGNAL(finished()), updateBannerThread, SLOT(quit()));
                        connect(updateBannerThread, SIGNAL(finished()), updateBanner, SLOT(deleteLater()));
                        connect(updateBannerThread, SIGNAL(finished()), updateBannerThread, SLOT(deleteLater()));

                        updateBanner->moveToThread(updateBannerThread);
                        updateBannerThread->start();
                        updateBanner->exec(tweet);
                    } else {
                        qWarning() << "[Warning] UpdateProfile: update_banner is disabled.";
                    }
                } else {
                    qWarning() << "[Warning] UpdateProfile: Unknown command:" << command;
                }
            }
        }
    }
    emit finished();
}

bool UpdateProfile::getScreenName()
{
    try {
        m_myscreenname = m_twitter.verifyCredentials().screen_name();
        m_settings.setScreenName(m_myscreenname);
        emit stateChanged(GetScreenNameFinished);
        qDebug() << "[Info] UpdateProfile: Gettings screen_name Successed.\n"
                    "       Your screen_name:" << m_myscreenname;
        return true;
    } catch(std::runtime_error&) {
        try {
            m_myscreenname = m_twitter.usersLookup(NULL, m_settings.userId()).screen_name();
            m_settings.setScreenName(m_myscreenname);
            emit stateChanged(GetScreenNameFinished);
            qDebug() << "[Info] UpdateProfile: Gettings screen_name Successed.\n"
                        "       Your screen_name:" << m_myscreenname;
            return true;
        } catch(const std::runtime_error &e) {
            qCritical() << "[Error] UpdateProfile: Getting screen_name Failed.\n"
                           "        Error Message:" << e.what();
            m_errormessage = QString::fromStdString(e.what());
            emit error(GetScreenNameFailed);
            return false;
        }
    }
}
