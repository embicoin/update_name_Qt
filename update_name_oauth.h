#ifndef UPDATE_NAME_OAUTH_H
#define UPDATE_NAME_OAUTH_H

#include <QDateTime>

#define OAUTH_SIGNATURE_METHOD "HMAC-SHA1"
#define OAUTH_TIMESTAMP QDateTime::currentMSecsSinceEpoch() / 1000
#define OAUTH_NONCE QDateTime::currentMSecsSinceEpoch()
#define OAUTH_VERSION "1.0"

#endif // UPDATE_NAME_OAUTH_H
