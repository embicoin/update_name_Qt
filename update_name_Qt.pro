#-------------------------------------------------
#
# Project created by QtCreator 2014-12-21T13:23:25
#
#-------------------------------------------------

QT       += core gui network
!android: QT += webkitwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = update_name_Qt
TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    preferencesdialog.cpp \
    twitterAPI/oauth/oauth.cpp \
    twitterAPI/rest/media/upload.cpp \
    twitterAPI/rest/statuses/update.cpp \
    twitterAPI/rest/restapi.cpp \
    twitterAPI/rest/account/updateprofile.cpp \
    twitterAPI/object/error.cpp \
    twitterAPI/object/tweets.cpp \
    twitterAPI/object/users.cpp \
    twitterAPI/streaming/user.cpp \
    twitterAPI/streaming/parsejson.cpp \
    updatename.cpp \
    twitterAPI/rest/users/lookup.cpp \
    twitterAPI/object/entities.cpp \
    twitterAPI/object/object.cpp \
    twitterAPI/rest/account/updateprofileimage.cpp \
    twitterAPI/rest/account/updateprofilebackgroundimage.cpp \
    twitterAPI/rest/account/updateprofilebanner.cpp

HEADERS  += mainwindow.h \
    twitterAPI/oauth/oauth.h \
    twitterAPI/twitterglobal.h \
    updatenameqtglobal.h \
    preferencesdialog.h \
    twitterAPI/rest/media/upload.h \
    twitterAPI/rest/statuses/update.h \
    twitterAPI/rest/restapi.h \
    twitterAPI/rest/account/updateprofile.h \
    twitterAPI/object/error.h \
    twitterAPI/object/tweets.h \
    twitterAPI/object/users.h \
    twitterAPI/streaming/user.h \
    twitterAPI/streaming/parsejson.h \
    updatename.h \
    twitterAPI/rest/users/lookup.h \
    twitterAPI/object/entities.h \
    twitterAPI/object/object.h \
    twitterAPI/rest/account/updateprofileimage.h \
    twitterAPI/rest/account/updateprofilebackgroundimage.h \
    twitterAPI/rest/account/updateprofilebanner.h

android {
    SOURCES  += android/authdialog.cpp
    HEADERS  += android/authdialog.h
    FORMS    += android/authdialog.ui \
                android/preferencesdialog.ui
} else {
    SOURCES  += authdialog.cpp
    HEADERS  += authdialog.h
    FORMS    += preferencesdialog.ui
    win32|win64 {
        FORMS    += windows/authdialog.ui
    } else {
        FORMS    += authdialog.ui
    }
}

FORMS    += mainwindow.ui

CONFIG += mobility
MOBILITY = 

DISTFILES +=

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

