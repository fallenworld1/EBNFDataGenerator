#-------------------------------------------------
#
# Project created by QtCreator 2016-02-10T18:05:31
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_datageneratortester
CONFIG   += console c++11
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_datageneratortester.cpp \
    src/tst_datageneratortester.cpp \
    ../../src/parser.cpp \
    ../../src/routines.cpp \
    ../../src/tokens.cpp \
    ../../src/tree.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"
INCLUDEPATH += $$PWD/../../src
