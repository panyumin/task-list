#-------------------------------------------------
#
# Project created by QtCreator 2012-02-12T01:03:46
#
#-------------------------------------------------

QT       += core gui network

QT       += xml

TARGET = tasklist
TEMPLATE = app

INCLUDEPATH += kqoauth/include kqoauth/src
LIBS += -Lkqoauth/lib -lkqoauthd0

SOURCES += main.cpp\
        mainwindow.cpp \
    task_dial.cpp \
    task_list.cpp \
    list_dial.cpp \
    sync_widget.cpp \


HEADERS  += mainwindow.h \
    task_dial.h \
    task_list.h \
    list_dial.h \
    sync_widget.h \


OTHER_FILES += \
    kqoauth/lib/libkqoauthd0.a \
    kqoauth/lib/kqoauthd0.dll \
    kqoauth/lib/kqoauthd.prl \
    kqoauth/lib/kqoauth.prl \
    kqoauth/src/src.pro \
    kqoauth/src/pcfile.sh \
    kqoauth/src/object_script.libkqoauthd.Static-Debug \
    kqoauth/src/object_script.libkqoauth.Static-Release \
    kqoauth/src/object_script.kqoauthd.Shared-Debug \
    kqoauth/src/object_script.kqoauth.Shared-Release \
    kqoauth/src/Makefile.StaticRelease \
    kqoauth/src/Makefile.StaticDebug \
    kqoauth/src/Makefile.SharedRelease \
    kqoauth/src/Makefile.SharedDebug \
    kqoauth/src/Makefile \
    kqoauth/src/kqoauthd_resource.rc \
    kqoauth/src/kqoauth_resource.rc \
    kqoauth/include/QtKOAuth \
    kqoauth/src/kqoauthutils.h \
    kqoauth/src/kqoauthrequest_xauth_p.h \
    kqoauth/src/kqoauthrequest_xauth.h \
    kqoauth/src/kqoauthrequest_p.h \
    kqoauth/src/kqoauthrequest_1.h \
    kqoauth/src/kqoauthrequest.h \
    kqoauth/src/kqoauthmanager_p.h \
    kqoauth/src/kqoauthmanager.h \
    kqoauth/src/kqoauthglobals.h \
    kqoauth/src/kqoauthauthreplyserver_p.h \
    kqoauth/src/kqoauthauthreplyserver.h \
    kqoauth/src/kqoauthutils.cpp \
    kqoauth/src/kqoauthrequest_xauth.cpp \
    kqoauth/src/kqoauthrequest_1.cpp \
    kqoauth/src/kqoauthrequest.cpp \
    kqoauth/src/kqoauthmanager.cpp \
    kqoauth/src/kqoauthauthreplyserver.cpp \
    style.qss

RESOURCES += \
    images.qrc
