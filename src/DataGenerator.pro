TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
SOURCES += \
    tokens.cpp \
    parser.cpp \
    routines.cpp \
    tree.cpp \
    main.cpp
include(deployment.pri)
qtcAddDeployment()

QMAKE_LFLAGS += -Wl,-rpath='$$ORIGIN'
HEADERS += \
    tree.h \
    parser.h \
    routines.h \
    tokens.
