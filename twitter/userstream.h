#ifndef USERSTREAM_H
#define USERSTREAM_H

#include "../settings.h"

#include <QThread>

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

signals:
    void stateChanged(UserStream::State state);
    void receivedData(const QByteArray &data);
    void stopping();

public slots:
    void stop();

protected:
    virtual void run();

private:
    static const QString USERSTREAM_URL;
    Settings m_settings;
    volatile bool m_isStopped;
};

#endif // USERSTREAM_H
