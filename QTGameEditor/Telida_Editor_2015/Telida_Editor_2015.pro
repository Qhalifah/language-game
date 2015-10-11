#-------------------------------------------------
#
# Project created by QtCreator 2015-05-31T11:16:30
#
#-------------------------------------------------

QT       += core gui
QT       += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Telida_Editor_2015
TEMPLATE = app
RC_FILE = resource.rc

SOURCES += main.cpp\
        mainwindow.cpp \
    word.cpp \
    screenqgv.cpp \
    scene.cpp \
    myrect.cpp \
    Map.cpp \
    mainWin_menu.cpp \
    mainwin_maindict.cpp \
    mainwin_dictset.cpp \
    mainwin_badges.cpp \
    maindictionary.cpp \
    handleitem.cpp \
    filemanager.cpp \
    dictionarysets.cpp \
    dictionary.cpp \
    DBMap.cpp \
    chooseFromList.cpp \
    chooseacttype.cpp \
    chooseactivity.cpp \
    badges.cpp \
    backgroundmusic.cpp \
    activity.cpp \
    activities.cpp \

HEADERS  += mainwindow.h \
    word.h \
    screenqgv.h \
    screen.h \
    scene.h \
    myrect.h \
    map.h \
    maindictionary.h \
    handleitem.h \
    filemanager.h \
    dictionarysets.h \
    dictionary.h \
    DBMap.h \
    chooseFromList.h \
    chooseacttype.h \
    chooseactivity.h \
    badges.h \
    badge.h \
    backgroundmusic.h \
    activity.h \
    activities.h \
    Communication.h

FORMS    += \
    chooseactivity.ui \
    chooseacttype.ui \
    choosefromlist.ui \
    mainwindow.ui

RESOURCES +=
