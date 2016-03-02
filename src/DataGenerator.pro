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
HEADERS += \
    tree.h \
    parser.h \
    routines.h \
    tokens. \
    tokens.h






unix:!macx|win32: LIBS += $$(THRDPARTY)/lib/boost/libboost_chrono.a
unix:!macx|win32: LIBS += $$(THRDPARTY)/lib/boost/libboost_system.a
unix:!macx|win32: LIBS += $$(THRDPARTY)/lib/boost/libboost_thread.a
INCLUDEPATH += $$(THRDPARTY)/include
DEPENDPATH += $$(THRDPARTY)/include

