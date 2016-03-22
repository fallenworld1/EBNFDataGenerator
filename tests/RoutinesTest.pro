#-------------------------------------------------
#
# Project created by QtCreator 2016-02-10T18:05:31
#
#-------------------------------------------------
TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle


SOURCES += \
    routine_tests.cpp \
    tokenstest.cpp \
    tree_tests.cpp \
    ../src/tokens.cpp \
    ../src/tree.cpp \
    ../src/routines.cpp \
    parser_tests.cpp \
    ../src/parser.cpp \
    ../src/policy.cpp \
    ../src/generator.cpp \
    ../src/treebuilder.cpp \
    generator_tests.cpp

INCLUDEPATH+= ../src


unix|win32: LIBS += -L$$(THRDPARTY)/gtest/lib/ -lgtest
unix|win32: LIBS += -L$$(THRDPARTY)/gtest/lib/ -lgtest_main

INCLUDEPATH +=$$(THRDPARTY)/gtest/include
DEPENDPATH  +=$$(THRDPARTY)/gtest/include


#unix:!macx|win32: LIBS += $$(THRDPARTY)/gtest/lib/-lgtest
#unix:!macx|win32: LIBS += $$(THRDPARTY)/gtest/lib/ -lgtest_main

#win32:!win32-g++: PRE_TARGETDEPS += $$PWD/C:/Tools/thrdparty/gtest/lib/gtest.lib
#else:unix:!macx|win32-g++: PRE_TARGETDEPS += $$PWD/C:/Tools/thrdparty/gtest/lib/libgtest.a
#win32:!win32-g++: PRE_TARGETDEPS += $$PWD/C:/Tools/thrdparty/gtest/lib/gtest_main.lib
#else:unix:!macx|win32-g++: PRE_TARGETDEPS += $$PWD/C:/Tools/thrdparty/gtest/lib/gtest_main.a
