#-------------------------------------------------
#
# Project created by QtCreator 2016-02-10T18:05:31
#
#-------------------------------------------------
TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle

SOURCES += \
    ../src/routines.cpp \
    main.cpp \
    routine_tests.cpp \
    tokenstest.cpp \
    ../src/tokens.cpp \
    ../src/tree.cpp

INCLUDEPATH+= ../src

unix|win32: LIBS +=  $$(THRDPARTY)/gtest/lib/libgtest.a

INCLUDEPATH += $$(THRDPARTY)/gtest/include
#message(INCLUDEPATH:    $$INCLUDEPATH)
DEPENDPATH += $$(THRDPARTY)/gtest/include
#message(DEPENDPATH:     $$DEPENDPATH)

#win32:!win32-g++: PRE_TARGETDEPS +=  $$(THRDPARTY)/gtest/lib/gtest.lib
#else:unix|win32-g++: PRE_TARGETDEPS +=  $$(THRDPARTY)/gtest/lib/libgtest.a
