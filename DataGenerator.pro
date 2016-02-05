TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    maintoken.cpp

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    basetoken.h \
    texttoken.h \
    tokens.h \
    concattoken.h \
    ortoken.h \
    squarebracetoken.h \
    figurebracetoken.h

