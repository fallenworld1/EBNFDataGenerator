TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    parser.cpp \
    tokens.cpp \
    tree.cpp

include(deployment.pri)
qtcAddDeployment()

INCLUDEPATH += $$PWD/../../boost/lib/include
DEPENDPATH += $$PWD/../../boost/lib/include

HEADERS += \
    parser.h \
    routines.h \
    tokens.h \
    tree.h

