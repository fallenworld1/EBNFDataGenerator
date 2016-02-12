#-------------------------------------------------
#
# Project created by QtCreator 2016-02-11T15:16:59
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tokenstest
CONFIG   += console c++11
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tokenstest.cpp \
    ../../src/tree.cpp \
    ../../src/routines.cpp \
    ../../src/tokens.cpp

INCLUDEPATH += ../../src/

