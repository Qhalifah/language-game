#-------------------------------------------------
#
# Project created by QtCreator 2014-03-15T12:33:35
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GameEditor
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    screenqgv.cpp \
    myrect.cpp \
    handleitem.cpp \
    backgroundmusic.cpp \
    filemanager.cpp \
    word.cpp \
    dictionary.cpp \
    choosefromlist.cpp \
    dbmap.cpp \
    mainwin_menu.cpp \
    maindictionary.cpp \
    dictionarysets.cpp \
    mainwin_maindict.cpp \
    mainwin_dictset.cpp \
    activities.cpp \
    chooseactivity.cpp \
    chooseacttype.cpp \
    activity.cpp \
    scene.cpp \
    Map.cpp \
    mainwin_badges.cpp \
    badges.cpp

HEADERS  += mainwindow.h \
    screenqgv.h \
    myrect.h \
    handleitem.h \
    backgroundmusic.h \
    ../SFML_BGM/BGM.h \
    ../SFML_BGM/Communication.h \
    filemanager.h \
    map.h \
    screen.h \
    dictionary.h \
    word.h \
    choosefromlist.h \
    badge.h \
    dbmap.h \
    activity.h \
    maindictionary.h \
    dictionarysets.h \
    activities.h \
    chooseactivity.h \
    chooseacttype.h \
    scene.h \
    badges.h


FORMS    += mainwindow.ui \
    choosefromlist.ui \
    chooseactivity.ui \
    chooseacttype.ui

CONFIG += c++11

INCLUDEPATH += include/ \
