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
    is_stopped = true;
}

void UserStream::run()
{
    const QByteArray signature_key = settings.consumerSecret().toUtf8()
            + "&"
            + settings.accessTokenSecret().toUtf8();
    QVariantMap oauth_params;
    QUrlQuery oauth_query;
    QByteArray oauth_header;
    QByteArray signature_base_string;
    QNetworkRequest request;
    QNetworkAccessManager manager;
    QByteArray response;
    QByteArray buffer;
    QNetworkReply *reply = 0;
    QEventLoop loop;
    bool connected = false;
    int failed_count = 0;

    is_stopped = false;

    while(!is_stopped && failed_count < 5) {
        emit stateChanged(Connecting);

        oauth_params["oauth_consumer_key"]     = settings.consumerKey();
        oauth_params["oauth_token"]            = settings.accessToken();
        oauth_params["oauth_signature_method"] = OAUTH_SIGNATURE_METHOD;
        oauth_params["oauth_timestamp"]        = OAUTH_TIMESTAMP;
        oauth_params["oauth_nonce"]            = OAUTH_NONCE;
        oauth_params["oauth_version"]          = OAUTH_VERSION;

        oauth_query.clear();
        for(int i = 0; i < oauth_params.size(); i++) {
            oauth_query.addQueryItem(oauth_params.keys()[i], oauth_params.values()[i].toString());
        }

        signature_base_string = "GET&"
                + USERSTREAM_URL.toUtf8().toPercentEncoding()
                + "&"
                + oauth_query.toString().toUtf8().toPercentEncoding();

        oauth_params["oauth_signature"] =
                QMessageAuthenticationCode::hash(signature_base_string,
                                                 signature_key,
                                                 QCryptographicHash::Sha1).toBase64().toPercentEncoding();

        oauth_header = "OAuth ";
        for(int i = 0; i < oauth_params.size(); i++) {
            oauth_header.append(oauth_params.keys()[i]
                               + "=\""
                               + oauth_params.values()[i].toByteArray()
                               + "\", ");
        }
        oauth_header.chop(2);

        request.setUrl(QUrl(USERSTREAM_URL));
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
        request.setRawHeader("Authorization", oauth_header);

        reply = manager.get(request);

        connect(reply, SIGNAL(readyRead()), &loop, SLOT(quit()));
        connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
        connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), &loop, SLOT(quit()));
        connect(this, SIGNAL(stopping()), &loop, SLOT(quit()));
        connect(this, SIGNAL(stopping()), reply, SLOT(abort()));

        buffer.clear();

        while(!reply->isFinished() && !is_stopped && reply->error() == QNetworkReply::NoError) {
            loop.exec();

            if(is_stopped) {
                emit stateChanged(DisConnected);
                return;
            } else {
                if(reply->error() == QNetworkReply::NoError) {
                    if(!connected) {
                        emit stateChanged(Running);
                        connected = true;
                        failed_count = 0;
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
                    failed_count++;
                }
            }
        }
        emit stateChanged(DisConnected);
        connected = false;

        if(failed_count < 5) {
            emit stateChanged(Waiting);
            sleep(5);
        }
    }

    reply->deleteLater();
}

void UserStream::stop()
{
    is_stopped = true;
    emit stopping();
}
