#-------------------------------------------------
#
# Project created by QtCreator 2013-03-22T17:27:43
#
#-------------------------------------------------

QT       += core gui sql webkit

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Access
TEMPLATE = app


SOURCES += main.cpp \
    window.cpp \
    changepassword.cpp \
    loginto.cpp \
    about.cpp

HEADERS  += \
    window.h \
    changepassword.h \
    loginto.h \
    delegate.h \
    about.h \
    mylistview.h

FORMS    += \
    window.ui \
    changepassword.ui \
    loginto.ui \
    about.ui

RESOURCES += \
    resource.qrc
RC_FILE = 1.rc
