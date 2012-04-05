TEMPLATE = lib
CONFIG   += create_prl
QT       -= gui
QT       += network
TARGET   = gtasks
DESTDIR  = $$PWD/../lib

VERSION = 0.1

win32:DLLDESTDIR = $${DESTDIR}
CONFIG(static) {
	DEFINES += GTASKS_STATIC_LIB
} else {
	DEFINES += MAKE_GTASKS_LIB
}

INCLUDEPATH += qjson/
LIBS += -Lqjson/ -lqjson0

INCLUDEPATH += simpleoauth/
LIBS += -Lsimpleoauth/ -lsimpleoauth0

DEFINES += SIMPLEOAUTH_STATIC_LIB

HEADERS += \
    gtasks_export.h \
    cache.h \
    job.h \
    jobs.h \
    service.h \
    task_p.h \
    task.h \
    taskcollection.h \
    taskcollection_p.h \
    tasklistcollection_p.h \
    tasklistcollection.h \
    tasklist_p.h \
    tasklist.h \
    error.h \
    error_p.h

SOURCES += \
    jobs.cpp \
    job.cpp \
    service.cpp \
    task.cpp \
    taskcollection.cpp \
    tasklistcollection.cpp \
    tasklist.cpp \
    error.cpp
