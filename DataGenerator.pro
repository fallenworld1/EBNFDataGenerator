TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    maintoken.cpp \
    parser.cpp \
    customtoken.cpp

include(deployment.pri)
qtcAddDeployment()

INCLUDEPATH += $$PWD/../../boost/lib/include
DEPENDPATH += $$PWD/../../boost/lib/include

HEADERS += \
    basetoken.h \
    texttoken.h \
    concattoken.h \
    ortoken.h \
    squarebracetoken.h \
    figurebracetoken.h \
    customtoken.h \
    parser.h \
    maintoken.h

