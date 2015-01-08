#include "updatenameqtglobal.h"

//using namespace UpdateNameQt;

QString UpdateNameQt::someString()
{
    const QString token("-=^~\\|@`[{:*]},<.>/?_!\"#$%&'()'");
    static int currentIndex = 0;
    static int outputLength = 1;
    QString s;

    for (int i = 0; i < outputLength; i++) {
        s.append(token.at(currentIndex++));
        if (currentIndex > token.length()) {
            currentIndex = 0;
            if (outputLength == 1)
                outputLength++;
            else
                outputLength--;
        }
    }
    return s;
}
