#-------------------------------------------------
#
# Project created by QtCreator 2016-02-10T18:05:31
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = routinestester
CONFIG   += console c++11
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += \
    routinestester.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"
INCLUDEPATH += $$PWD/../../src
INCLUDEPATH += $$PWD/..
