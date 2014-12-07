#-------------------------------------------------
#
# Project created by QtCreator 2014-11-10T13:49:56
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = update_name_Qt
TEMPLATE = app

CONFIG += c++11

mac: ICON = upate_name_Qt_Mac.icns
win32: RC_FILE = WindowsApplicationIcon.rc

SOURCES += main.cpp\
        mainwindow.cpp \
    authdialog.cpp \
    settings.cpp \
    preferencesdialog.cpp \
    updatenamesender.cpp \
    updateprofile.cpp \
    update/name.cpp \
    update/update.cpp \
    twitter/restclient.cpp \
    twitter/tweetobject.cpp \
    twitter/usersobject.cpp \
    twitter/userstream.cpp \
    update/url.cpp \
    update/location.cpp \
    update/description.cpp \
    update/image.cpp \
    twitter/entitiesobject.cpp \
    update/backgroundimage.cpp

HEADERS  += mainwindow.h \
    authdialog.h \
    update_name_oauth.h \
    settings.h \
    preferencesdialog.h \
    updatenamesender.h \
    updateprofile.h \
    update/name.h \
    update/update.h \
    twitter/restclient.h \
    twitter/tweetobject.h \
    twitter/usersobject.h \
    twitter/userstream.h \
    update/url.h \
    update/location.h \
    update/description.h \
    update/image.h \
    twitter/entitiesobject.h \
    update/backgroundimage.h \
    update_nane_qt_global.h

FORMS    += mainwindow.ui \
    authdialog.ui \
    preferencesdialog.ui \
    updatenamesender.ui

RESOURCES += \
    icons.qrc

OTHER_FILES += \
    update_name_icon.xcf \
    update_name_Qt_Windows.ico \
    README.md \
    LICENSE \
    WindowsApplicationIcon.rc
