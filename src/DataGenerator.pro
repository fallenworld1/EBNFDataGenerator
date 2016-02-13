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



unix|win32: LIBS += -L$$PWD/../../../../boost/lib/lib/ -lboost_chrono
unix|win32: LIBS += -L$$PWD/../../../../boost/lib/lib/ -lboost_system
unix|win32: LIBS += -L$$PWD/../../../../boost/lib/lib/ -lboost_thread

INCLUDEPATH += $$PWD/../../../../boost/lib/include
DEPENDPATH += $$PWD/../../../../boost/lib/include

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../../../../boost/lib/lib/boost_chrono.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$PWD/../../../../boost/lib/lib/libboost_chrono.a
win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../../../../boost/lib/lib/boost_system.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$PWD/../../../../boost/lib/lib/libboost_system.a
win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../../../../boost/lib/lib/boost_thread.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$PWD/../../../../boost/lib/lib/libboost_thread.a
