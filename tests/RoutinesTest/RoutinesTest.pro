#-------------------------------------------------
#
# Project created by QtCreator 2016-02-10T18:05:31
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = RoutinesTester
CONFIG   += console\
            c++11
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += \
    routinestester.cpp \
    ../../src/routines.cpp
INCLUDEPATH+= ../../src/
