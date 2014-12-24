#ifndef UPDATENAMEQTGLOBAL
#define UPDATENAMEQTGLOBAL

#include <QVariantMap>
#include <QSettings>

extern QSettings *settings;

namespace UpdateNameQt {
const QVariantMap defaultSettings = {
    {"ConsumerKey", "1cIrbCiqwd3iRV5EZVHiHnzA0"},
    {"ConsumerSecret", "VYTYLtKC1p8bNUiakIEMiAvQTRSXsvRo7nBv7Srh5hvmmX7Pf9"},
};

const QStringList updateCommands = QStringList() << "update_name"
                                                 << "update_url"
                                                 << "update_location"
                                                 << "update_description"
                                                 << "update_image"
                                                 << "update_background"
                                                 << "update_banner";

enum ReturnCode {
    ExitSucceess = 0,
    ExitFaileure = 1,
    ExitRestart  = 1000,
};

extern QSettings *settings;
}

#endif // UPDATENAMEQTGLOBAL

