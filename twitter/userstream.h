#ifndef USERSTREAM_H
#define USERSTREAM_H

#include "../settings.h"

#include <QThread>
#include <QNetworkReply>

class UserStream : public QThread
{
    Q_OBJECT
public:
    explicit UserStream(QObject *parent = 0);

    enum State {
        ConnectionFailed,
        Running,
        Connecting,
        DisConnected,
        Waiting,
    };

    QString errorString();
    int waitTime();

signals:
    void stateChanged(UserStream::State state);
    void receivedData(const QByteArray &data);
    void stopping();

public slots:
    void stop();

protected:
    void run();

private:
    static const QString USERSTREAM_URL;
    Settings m_settings;
    QString m_errorMessage;
    int m_waitTime = 1;
    volatile bool m_isStopped;
};

#endif // USERSTREAM_H
