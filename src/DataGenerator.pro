TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    src/main.cpp \
    src/parser.cpp \
    src/routines.cpp \
    src/tokens.cpp \
    src/tree.cpp \
    tokens.cpp \
    parser.cpp \
    routines.cpp \
    tree.cpp \
    main.cpp
include(deployment.pri)
qtcAddDeployment()


HEADERS += \
    src/parser.h \
    src/routines.h \
    src/tokens.h \
    src/tree.h \
    tree.h \
    parser.h \
    routines.h \
    tokens.h

