QT += core
QT -= gui

CONFIG += c++11

TARGET = verk1
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    person.cpp \
    personservice.cpp \
    personpresentation.cpp

HEADERS += \
    person.h \
    personservice.h \
    personpresentation.h
