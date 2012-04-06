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
  
PRIVATE_HEADERS += \
	task_p.h \  
    taskcollection_p.h \
    tasklistcollection_p.h \    
    tasklist_p.h \   
    error_p.h

PUBLIC_HEADERS  += \
    gtasks_export.h \
    cache.h \
    job.h \
    jobs.h \
    service.h \
    tasklistcollection.h \
    tasklist.h \
    error.h \
    task.h \
    taskcollection.h 
    
SOURCES += \
    jobs.cpp \
    job.cpp \
    service.cpp \
    task.cpp \
    taskcollection.cpp \
    tasklistcollection.cpp \
    tasklist.cpp \
    error.cpp
  
HEADERS += $$PRIVATE_HEADERS $$PUBLIC_HEADERS
  
headers.files = $$PUBLIC_HEADERS
headers.path = $$OUT_PWD/../include/gtasks
INSTALLS += headers
