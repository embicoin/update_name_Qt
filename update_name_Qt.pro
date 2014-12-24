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
    preferencesdialog.cpp

HEADERS  += mainwindow.h \
    twitterAPI/oauth/oauth.h \
    twitterAPI/twitterglobal.h \
    updatenameqtglobal.h \
    preferencesdialog.h

# Androidとそれ以外で分ける
android {
    SOURCES  += android/authdialog.cpp
    HEADERS  += android/authdialog.h
    FORMS    += android/authdialog.ui
} else {
    SOURCES  += authdialog.cpp
    HEADERS  += authdialog.h
    FORMS    += authdialog.ui
}

FORMS    += mainwindow.ui \
    preferencesdialog.ui

CONFIG += mobility
MOBILITY = 

