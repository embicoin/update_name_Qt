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
    twitterAPI/oauth/oauth.cpp \
    preferencesdialog.cpp \
    twitterAPI/rest/media/upload.cpp \
    twitterAPI/rest/statuses/update.cpp \
    twitterAPI/rest/restapi.cpp \
    twitterAPI/rest/account/updateprofile.cpp \
    twitterAPI/object/error.cpp \
    twitterAPI/object/tweets.cpp \
    twitterAPI/object/users.cpp \
    twitterAPI/streaming/user.cpp \
    twitterAPI/streaming/parsejson.cpp

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
    twitterAPI/streaming/parsejson.h

android {
    SOURCES  += android/authdialog.cpp
    HEADERS  += android/authdialog.h
    FORMS    += android/authdialog.ui
} else {
    SOURCES  += authdialog.cpp
    HEADERS  += authdialog.h
    win32|win64 {
        FORMS    += windows/authdialog.ui
    } else {
        FORMS    += authdialog.ui
    }
}

FORMS    += mainwindow.ui \
    preferencesdialog.ui

CONFIG += mobility
MOBILITY = 

