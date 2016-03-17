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
    ../src/parser.cpp \
    ../src/policy.cpp \
    ../src/generator.cpp \
    generator_tests.cpp

INCLUDEPATH+= ../src






unix|win32: LIBS += -lgtest
unix|win32: LIBS += -lgtest_main
