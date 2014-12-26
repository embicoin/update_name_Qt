#ifndef UPDATENAMEQTGLOBAL
#define UPDATENAMEQTGLOBAL

#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif

#include <QVariantMap>
#include <QSettings>

namespace UpdateNameQt {

enum Over20CharName {
    ChopName,
    CancelUpdateName,
};
enum ReturnCode {
    ExitSucceess = 0,
    ExitFaileure = 1,
    ExitRestart  = 1000,
};

const QVariantMap defaultSettings = {
    {"ConsumerKey", "1cIrbCiqwd3iRV5EZVHiHnzA0"},
    {"ConsumerSecret", "VYTYLtKC1p8bNUiakIEMiAvQTRSXsvRo7nBv7Srh5hvmmX7Pf9"},
    {"AccessToken", ""},
    {"AccessTokenSecret", ""},

    {"StaysOnSystemTray", false},
    {"RetryStatusUpdateOnStatusIsADuplicate", true},
    {"SaveUpdateHistory", false},
    {"UpdateHistoryFileName", ""},

    {"EnabledUpdateName", true},
    {"EnabledUpdateNameFormat", false},
    {"UpdateNameFormat", ""},
    {"Over20CharName", ChopName},

    {"EnabledUpdateUrl", true},
    {"EnabledCheckUrl", false},

    {"EnabledUpdateLocation", true},

    {"EnabledUpdateDescription", true},

    {"EnabledUpdateImage", true},

    {"EnabledUpdateBackground", true},

    {"EnabledUpdateBanner", true},

    {"EnabledStartupMessage", true},
    {"StartupMessage", "update_nameを起動しました。"},
    {"EnabledQuitMessage", true},
    {"QuitMessage", "update_nameを終了しました。"},

    {"EnabledUpdateNameSuccessedMessage", true},
    {"UpdateNameSuccessedMessage", "@%{screen_name} nameを\"%{name}\"に変更しました。"},
    {"EnabledUpdateNameFailedMessage", true},
    {"UpdateNameFailedMessage", "@%{screen_name} nameの変更に失敗しました。\n%{error}"},

    {"EnabledUpdateUrlSuccessedMessage", true},
    {"UpdateUrlSuccessedMessage", "@%{screen_name} urlを\"%{url}\"に変更しました。"},
    {"EnabledUpdateUrlFailedMessage", true},
    {"UpdateUrlFailedMessage", "@%{screen_name} urlの変更に失敗しました。\n%{error}"},

    {"EnabledUpdateLocationSuccessedMessage", true},
    {"UpdateLocationSuccessedMessage", "@%{screen_name} locationを\"%{location}\"に変更しました。"},
    {"EnabledUpdateLocationFailedMessage", true},
    {"UpdateLocationFailedMessage", "@%{screen_name} locationの変更に失敗しました。\n%{error}"},

    {"EnabledUpdateDescriptionSuccessedMessage", true},
    {"UpdateDescriptionSuccessedMessage", "@%{screen_name} descriptionを\"%{description}\"に変更しました。"},
    {"EnabledUpdateDescriptionFailedMessage", true},
    {"UpdateDescriptionFailedMessage", "@%{screen_name} descriptionの変更に失敗しました。\n%{error}"},

    {"EnabledUpdateImageSuccessedMessage", true},
    {"UpdateImageSuccessedMessage", "@%{screen_name} imageを\"%{image}\"に変更しました。"},
    {"EnabledUpdateImageFailedMessage", true},
    {"UpdateImageFailedMessage", "@%{screen_name} imageの変更に失敗しました。\n%{error}"},

    {"EnabledUpdateBackgroundSuccessedMessage", true},
    {"UpdateBackgroundSuccessedMessage", "@%{screen_name} backgroundを\"%{background}\"に変更しました。"},
    {"EnabledUpdateBackgroundFailedMessage", true},
    {"UpdateBackgroundFailedMessage", "@%{screen_name} backgroundの変更に失敗しました。\n%{error}"},

    {"EnabledUpdateImageSuccessedMessage", true},
    {"UpdateImageSuccessedMessage", "@%{screen_name} imageを\"%{image}\"に変更しました。"},
    {"EnabledUpdateImageFailedMessage", true},
    {"UpdateImageFailedMessage", "@%{screen_name} imageの変更に失敗しました。\n%{error}"},

    {"EnabledUpdateBannerSuccessedMessage", true},
    {"UpdateBannerSuccessedMessage", "@%{screen_name} bannerを\"%{banner}\"に変更しました。"},
    {"EnabledUpdateBannerFailedMessage", true},
    {"UpdateBannerFailedMessage", "@%{screen_name} bannerの変更に失敗しました。\n%{error}"},
};
const QStringList updateCommands = QStringList() << "update_name"
                                                 << "update_url"
                                                 << "update_location"
                                                 << "update_description"
                                                 << "update_image"
                                                 << "update_background"
                                                 << "update_banner";
extern QSettings *settings;
}

#endif // UPDATENAMEQTGLOBAL

