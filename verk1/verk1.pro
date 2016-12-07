QT += core
QT -= gui

CONFIG += c++11

TARGET = verk1
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    DataAccess/dataaccess.cpp \
    Models/person.cpp \
    UI/personpresentation.cpp \
    Service/personservice.cpp \
    Utilities/utils.cpp \
    DataAccess/personrepository.cpp

HEADERS += \
    DataAccess/dataaccess.h \
    Models/person.h \
    Service/personservice.h \
    UI/personpresentation.h \
    Utilities/utils.h \
    DataAccess/personrepository.h

QT += sql
