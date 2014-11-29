#include "userstream.h"
#include "../update_name_oauth.h"

#include <QUrlQuery>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QTimer>
#include <QMessageAuthenticationCode>
#include <QCryptographicHash>
#include <memory>

const QString UserStream::USERSTREAM_URL = "https://userstream.twitter.com/1.1/user.json";

UserStream::UserStream(QObject *parent) :
    QThread(parent)
{
    m_isStopped = true;
}

QString UserStream::errorString()
{
    return m_errorMessage;
}

int UserStream::waitTime()
{
    return m_waitTime;
}

void UserStream::run()
{
    m_isStopped = false;

    while(!m_isStopped) {
        const QByteArray signatureKey = m_settings.consumerSecret().toUtf8()
                + "&"
                + m_settings.accessTokenSecret().toUtf8();
        QVariantMap oauthParams;
        QUrlQuery oauth_query;
        QByteArray oauthHeader;
        QByteArray signatureBaseString;
        QNetworkRequest request;
        QNetworkAccessManager manager;
        std::unique_ptr<QNetworkReply> reply;
        QByteArray response;
        QByteArray buffer;
        QEventLoop loop;
        QTimer timer;
        bool connected = false;
        QEventLoop waitLoop;
        QTimer waitTimer;

        emit stateChanged(Connecting);

        oauthParams["oauth_consumer_key"]     = m_settings.consumerKey();
        oauthParams["oauth_token"]            = m_settings.accessToken();
        oauthParams["oauth_signature_method"] = OAUTH_SIGNATURE_METHOD;
        oauthParams["oauth_timestamp"]        = OAUTH_TIMESTAMP;
        oauthParams["oauth_nonce"]            = OAUTH_NONCE;
        oauthParams["oauth_version"]          = OAUTH_VERSION;

        for(int i = 0; i < oauthParams.size(); i++) {
            oauth_query.addQueryItem(oauthParams.keys()[i], oauthParams.values()[i].toString());
        }

        signatureBaseString = "GET&"
                + USERSTREAM_URL.toUtf8().toPercentEncoding()
                + "&"
                + oauth_query.toString().toUtf8().toPercentEncoding();

        oauthParams["oauth_signature"] =
                QMessageAuthenticationCode::hash(signatureBaseString,
                                                 signatureKey,
                                                 QCryptographicHash::Sha1).toBase64().toPercentEncoding();

        oauthHeader = "OAuth ";
        for(int i = 0; i < oauthParams.size(); i++) {
            oauthHeader.append(oauthParams.keys()[i]
                               + "=\""
                               + oauthParams.values()[i].toByteArray()
                               + "\", ");
        }
        oauthHeader.chop(2);

        request.setUrl(QUrl(USERSTREAM_URL));
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
        request.setRawHeader("Authorization", oauthHeader);

        reply.reset(manager.get(request));

        connect(reply.get(), SIGNAL(readyRead()), &loop, SLOT(quit()));
        connect(reply.get(), SIGNAL(finished()), &loop, SLOT(quit()));
        connect(reply.get(), SIGNAL(error(QNetworkReply::NetworkError)), &loop, SLOT(quit()));
        connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
        connect(this, SIGNAL(stopping()), &loop, SLOT(quit()));
        connect(this, SIGNAL(stopping()), reply.get(), SLOT(abort()));

        timer.setSingleShot(true);
        buffer.clear();

        while(true) {
            timer.start(60000);
            loop.exec();
            if(m_isStopped) {
                qDebug() << "[Info] UserStream: Stopping userstream.";
                emit stateChanged(DisConnected);
                return;
            } else if(reply->error() != QNetworkReply::NoError
                      && reply->error() != QNetworkReply::OperationCanceledError) {
                            m_errorMessage = reply->errorString();
                            qCritical() << "[Error] UserStream: Connection failed.\n"
                                           "        Error message:" << m_errorMessage;
                            if(connected) {
                                emit stateChanged(DisConnected);
                            }
                            emit stateChanged(ConnectionFailed);
                            break;
            } else if(!timer.isActive() || reply->isFinished()) {
                qWarning() << "[Warning] UserStream: Disconnected from userstream.";
                emit stateChanged(DisConnected);
                break;
            } else {
                if(!connected) {
                    qDebug() << "[Info] UserStream: Connected.";
                    emit stateChanged(Running);
                    connected = true;
                    m_waitTime = 1;
                }
                response = reply->readAll();

                if(response.indexOf("\r") > 0) {
                    if(!buffer.isEmpty()) {
                        response.prepend(QString::fromUtf8(buffer).split("\r").first().toUtf8());
                        buffer.replace(QString::fromUtf8(buffer).split("\r").first().toUtf8(), "");
                    }
                    emit receivedData(response);
                } else if(response.indexOf("\r") == -1) {
                    buffer.append(response);
                }
            }
        }
        connected = false;
        emit stateChanged(Waiting);

        connect(&waitTimer, SIGNAL(timeout()), &waitLoop, SLOT(quit()));
        connect(this, SIGNAL(stopping()), &waitLoop, SLOT(quit()));
        waitTimer.setSingleShot(true);
        waitTimer.start(m_waitTime * 1000);
        waitLoop.exec();
        m_waitTime *= 2;
    }
}

void UserStream::stop()
{
    m_isStopped = true;
    emit stopping();
}
