#ifndef TWITTERGLOBAL
#define TWITTERGLOBAL

#include <QDateTime>

#define OAUTH_NONCE QDateTime::currentMSecsSinceEpoch()
#define OAUTH_SIGNATURE_METHOD "HMAC-SHA1";
#define OAUTH_TIMESTAMP (QDateTime::currentMSecsSinceEpoch() / 1000)
#define OAUTH_VERSION "1.0"
#define TIMEOUT_COUNT_MSEC 30000

namespace TwitterAPI {
enum triBool {
    True = 1,
    False = 0,
    Null = -1,
};
}

#endif // TWITTERGLOBAL

