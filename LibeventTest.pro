TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -L/usr/local/ -levent

SOURCES += main.cpp \
    hello.cpp

