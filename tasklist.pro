#-------------------------------------------------
#
# Project created by QtCreator 2012-02-12T01:03:46
#
#-------------------------------------------------

QT       += core gui network

QT       += xml webkit

TARGET = tasklist
TEMPLATE = app

INCLUDEPATH += qjson/include/QJson qjson/src
LIBS += -Lqjson/lib -lqjson0

INCLUDEPATH += simpleoauth/include/simpleoauth simpleoauth/src
LIBS += -Lsimpleoauth/lib -lsimpleoauth0

INCLUDEPATH += gtasks/include/gtasks gtasks/src
LIBS += -Lgtasks/lib -lgtasks0

SOURCES += main.cpp\
        mainwindow.cpp \
    task_dial.cpp \
    task_list.cpp \
    list_dial.cpp \
    sync_widget.cpp \
    sync_server.cpp \
    htmleditor.cpp \
    search.cpp \
    search_dial.cpp



HEADERS  += mainwindow.h \
    task_dial.h \
    task_list.h \
    list_dial.h \
    sync_widget.h \
    sync_server.h \
    htmleditor.h \
    html_delegate.h \
    search.h \
    search_dial.h


OTHER_FILES += \
    style.qss \
    qjson/lib/qjson0.dll \
    qjson/lib/qjson.prl \
    qjson/lib/libqjson0.a \
    qjson/src/src.pro \
    qjson/src/qjson_resource.rc \
    qjson/src/Makefile.Release \
    qjson/src/Makefile.Debug \
    qjson/src/Makefile \
    qjson/src/json_parser.yy \
    qjson/src/CMakeLists.txt \
    qjson/src/.gitignore \
    qjson/src/serializerrunnable.cpp \
    qjson/src/serializer.cpp \
    qjson/src/qobjecthelper.cpp \
    qjson/src/parserrunnable.cpp \
    qjson/src/parser.cpp \
    qjson/src/json_scanner.cpp \
    qjson/src/json_parser.cc \
    qjson/include/QJson/Serializer \
    qjson/include/QJson/QObjectHelper \
    qjson/include/QJson/Parser \
    qjson/src/stack.hh \
    qjson/src/serializerrunnable.h \
    qjson/src/serializer.h \
    qjson/src/qobjecthelper.h \
    qjson/src/qjson_export.h \
    qjson/src/qjson_debug.h \
    qjson/src/position.hh \
    qjson/src/parserrunnable.h \
    qjson/src/parser_p.h \
    qjson/src/parser.h \
    qjson/src/location.hh \
    qjson/src/json_scanner.h \
    qjson/src/json_parser.hh \
    simpleoauth/src/src.pro \
    simpleoauth/src/simpleoauth_resource.rc \
    simpleoauth/src/Makefile.StaticRelease \
    simpleoauth/src/Makefile.StaticDebug \
    simpleoauth/src/Makefile.SharedRelease \
    simpleoauth/src/Makefile.SharedDebug \
    simpleoauth/src/Makefile.Release \
    simpleoauth/src/Makefile.Debug \
    simpleoauth/src/Makefile \
    simpleoauth/lib/simpleoauth0.dll \
    simpleoauth/lib/simpleoauth.prl \
    simpleoauth/lib/libsimpleoauth0.a \
    simpleoauth/lib/libsimpleoauth.a \
    simpleoauth/src/oauth_token.cpp \
    simpleoauth/src/oauth_helper.cpp \
    simpleoauth/src/simpleoauth_export.h \
    simpleoauth/src/oauth_token_p.h \
    simpleoauth/src/oauth_token.h \
    simpleoauth/src/oauth_helper.h \
    simpleoauth/include/simpleoauth/simpleoauth_export.h \
    simpleoauth/include/simpleoauth/oauth_token.h \
    simpleoauth/include/simpleoauth/oauth_helper.h \
    gtasks/lib/libgtasks0.a \
    gtasks/lib/gtasks0.dll \
    gtasks/lib/gtasks.prl \
    gtasks/src/src.pro \
    gtasks/src/object_script.gtasks.Release \
    gtasks/src/object_script.gtasks.Debug \
    gtasks/src/Makefile.Release \
    gtasks/src/Makefile.Debug \
    gtasks/src/Makefile \
    gtasks/src/gtasks_resource.rc \
    gtasks/src/tasklistcollection.cpp \
    gtasks/src/tasklist.cpp \
    gtasks/src/taskcollection.cpp \
    gtasks/src/task.cpp \
    gtasks/src/service.cpp \
    gtasks/src/jobs.cpp \
    gtasks/src/job.cpp \
    gtasks/src/error.cpp \
    gtasks/include/gtasks/tasklistcollection.h \
    gtasks/include/gtasks/tasklist.h \
    gtasks/include/gtasks/taskcollection.h \
    gtasks/include/gtasks/task.h \
    gtasks/include/gtasks/service.h \
    gtasks/include/gtasks/jobs.h \
    gtasks/include/gtasks/job.h \
    gtasks/include/gtasks/gtasks_export.h \
    gtasks/include/gtasks/error.h \
    gtasks/include/gtasks/cache.h \
    gtasks/src/tasklistcollection_p.h \
    gtasks/src/tasklistcollection.h \
    gtasks/src/tasklist_p.h \
    gtasks/src/tasklist.h \
    gtasks/src/taskcollection_p.h \
    gtasks/src/taskcollection.h \
    gtasks/src/task_p.h \
    gtasks/src/task.h \
    gtasks/src/service.h \
    gtasks/src/jobs.h \
    gtasks/src/job.h \
    gtasks/src/gtasks_export.h \
    gtasks/src/error_p.h \
    gtasks/src/error.h \
    gtasks/src/cache.h \
    gtasks/src/qjson/src.pro \
    gtasks/src/qjson/qjson0.dll \
    gtasks/src/qjson/qjson_resource.rc \
    gtasks/src/qjson/qjson.prl \
    gtasks/src/qjson/Makefile.Release \
    gtasks/src/qjson/Makefile.Debug \
    gtasks/src/qjson/Makefile \
    gtasks/src/qjson/libqjson0.a \
    gtasks/src/qjson/json_parser.yy \
    gtasks/src/qjson/CMakeLists.txt \
    gtasks/src/qjson/.gitignore \
    gtasks/src/simpleoauth/src.pro \
    gtasks/src/simpleoauth/simpleoauth0.dll \
    gtasks/src/simpleoauth/simpleoauth_resource.rc \
    gtasks/src/simpleoauth/simpleoauth.prl \
    gtasks/src/simpleoauth/Makefile.StaticRelease \
    gtasks/src/simpleoauth/Makefile.StaticDebug \
    gtasks/src/simpleoauth/Makefile.SharedRelease \
    gtasks/src/simpleoauth/Makefile.SharedDebug \
    gtasks/src/simpleoauth/Makefile.Release \
    gtasks/src/simpleoauth/Makefile.Debug \
    gtasks/src/simpleoauth/Makefile \
    gtasks/src/simpleoauth/libsimpleoauth0.a \
    gtasks/src/simpleoauth/libsimpleoauth.a \
    gtasks/src/qjson/serializerrunnable.cpp \
    gtasks/src/qjson/serializer.cpp \
    gtasks/src/qjson/qobjecthelper.cpp \
    gtasks/src/qjson/parserrunnable.cpp \
    gtasks/src/qjson/parser.cpp \
    gtasks/src/qjson/json_scanner.cpp \
    gtasks/src/qjson/json_parser.cc \
    gtasks/src/simpleoauth/oauth_token.cpp \
    gtasks/src/simpleoauth/oauth_helper.cpp \
    gtasks/src/qjson/stack.hh \
    gtasks/src/qjson/serializerrunnable.h \
    gtasks/src/qjson/serializer.h \
    gtasks/src/qjson/Serializer \
    gtasks/src/qjson/qobjecthelper.h \
    gtasks/src/qjson/QObjectHelper \
    gtasks/src/qjson/qjson_export.h \
    gtasks/src/qjson/qjson_debug.h \
    gtasks/src/qjson/position.hh \
    gtasks/src/qjson/parserrunnable.h \
    gtasks/src/qjson/parser_p.h \
    gtasks/src/qjson/parser.h \
    gtasks/src/qjson/Parser \
    gtasks/src/qjson/location.hh \
    gtasks/src/qjson/json_scanner.h \
    gtasks/src/qjson/json_parser.hh \
    gtasks/src/simpleoauth/simpleoauth_export.h \
    gtasks/src/simpleoauth/oauth_token_p.h \
    gtasks/src/simpleoauth/oauth_token.h \
    gtasks/src/simpleoauth/oauth_helper.h \
    images/undo.png \
    images/underline.png \
    images/strikethrough.png \
    images/save.png \
    images/right.png \
    images/redo.png \
    images/paste.png \
    images/open.png \
    images/new.png \
    images/left.png \
    images/italic.png \
    images/cut.png \
    images/copy.png \
    images/center.png \
    images/bold.png \
    images/number.png \
    images/bullet.png \
    images/indent_right.png \
    images/indent_left.png

RESOURCES += \
    images.qrc
