#include "userstream.h"
#include "update_name_oauth.h"

#include <QUrlQuery>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QTimer>
#include <QMessageAuthenticationCode>
#include <QCryptographicHash>

const QString UserStream::USERSTREAM_URL = "https://userstream.twitter.com/1.1/user.json";

UserStream::UserStream(QObject *parent) :
    QThread(parent)
{
    m_isStopped = true;
}

void UserStream::run()
{
    const QByteArray signatureKey = m_settings.consumerSecret().toUtf8()
            + "&"
            + m_settings.accessTokenSecret().toUtf8();
    QVariantMap oauthParams;
    QUrlQuery oauth_query;
    QByteArray oauthHeader;
    QByteArray signatureBaseString;
    QNetworkRequest request;
    QNetworkAccessManager manager;
    QByteArray response;
    QByteArray buffer;
    QNetworkReply *reply = 0;
    QEventLoop loop;
    bool connected = false;
    int failedCount = 0;

    m_isStopped = false;

    while(!m_isStopped && failedCount < 5) {
        emit stateChanged(Connecting);

        oauthParams["oauth_consumer_key"]     = m_settings.consumerKey();
        oauthParams["oauth_token"]            = m_settings.accessToken();
        oauthParams["oauth_signature_method"] = OAUTH_SIGNATURE_METHOD;
        oauthParams["oauth_timestamp"]        = OAUTH_TIMESTAMP;
        oauthParams["oauth_nonce"]            = OAUTH_NONCE;
        oauthParams["oauth_version"]          = OAUTH_VERSION;

        oauth_query.clear();
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

        reply = manager.get(request);

        connect(reply, SIGNAL(readyRead()), &loop, SLOT(quit()));
        connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
        connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), &loop, SLOT(quit()));
        connect(this, SIGNAL(stopping()), &loop, SLOT(quit()));
        connect(this, SIGNAL(stopping()), reply, SLOT(abort()));

        buffer.clear();

        while(!reply->isFinished() && !m_isStopped && reply->error() == QNetworkReply::NoError) {
            loop.exec();

            if(m_isStopped) {
                emit stateChanged(DisConnected);
                return;
            } else {
                if(reply->error() == QNetworkReply::NoError) {
                    if(!connected) {
                        emit stateChanged(Running);
                        connected = true;
                        failedCount = 0;
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
                } else {
                    emit stateChanged(ConnectionFailed);
                    failedCount++;
                }
            }
        }
        emit stateChanged(DisConnected);
        connected = false;

        if(failedCount < 5) {
            emit stateChanged(Waiting);
            sleep(5);
        }
    }

    reply->deleteLater();
}

void UserStream::stop()
{
    m_isStopped = true;
    emit stopping();
}
