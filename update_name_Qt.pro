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
    aboutdialog.cpp \
    preferencesdialog.cpp \
    userstream.cpp \
    twitter.cpp \
    updatename.cpp \
    updatenamesender.cpp

HEADERS  += mainwindow.h \
    authdialog.h \
    update_name_oauth.h \
    settings.h \
    aboutdialog.h \
    preferencesdialog.h \
    userstream.h \
    twitter.h \
    updatename.h \
    updatenamesender.h

FORMS    += mainwindow.ui \
    authdialog.ui \
    aboutdialog.ui \
    preferencesdialog.ui \
    updatenamesender.ui

RESOURCES += \
    icons.qrc

OTHER_FILES += \
    update_name_icon.xcf \
    update_name_Qt_Windows.ico \
    README.md \
    LICENSE
