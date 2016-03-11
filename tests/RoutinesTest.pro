#-------------------------------------------------
#
# Project created by QtCreator 2016-02-10T18:05:31
#
#-------------------------------------------------
TEMPLATE = app
CONFIG += console c++11 no_batch
CONFIG -= app_bundle


SOURCES += \
    routine_tests.cpp \
    tokenstest.cpp \
    tree_tests.cpp \
    ../src/tokens.cpp \
    ../src/tree.cpp \
    ../src/routines.cpp \
    parser_tests.cpp \
    ../src/parser.cpp

INCLUDEPATH+= ../src

INCLUDEPATH += $$(THRDPARTY)/include

DEPENDPATH += $$(THRDPARTY)/include

unix:!macx|win32: LIBS += -L$$(THRDPARTY)/lib/ -lgtest_64
unix:!macx|win32: LIBS += -L$$(THRDPARTY)/lib/ -lgtest_main_64
#libraries must be compilled with MTd flag
#to use Qt Creator Google Test plugin uncheck Projects->Run->Run in terminal

#win32:!win32-g++: PRE_TARGETDEPS += $$(THRDPARTY)/lib/gtest.lib
#else:unix:!macx|win32-g++: PRE_TARGETDEPS += $$(THRDPARTY)/lib/libgtest.a


