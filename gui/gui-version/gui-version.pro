#-------------------------------------------------
#
# Project created by QtCreator 2016-12-14T01:46:24
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gui-version
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    DataAccess/computerrepository.cpp \
    DataAccess/dataaccess.cpp \
    DataAccess/personrepository.cpp \
    Models/computer.cpp \
    Models/person.cpp \
    Service/personservice.cpp \
    Utilities/utils.cpp \
    Presentation/guipresentation.cpp \
    DataAccess/db.cpp \
    dialogadd.cpp

HEADERS  += mainwindow.h \
    DataAccess/computerrepository.h \
    DataAccess/dataaccess.h \
    DataAccess/personrepository.h \
    Models/computer.h \
    Models/person.h \
    Service/personservice.h \
    Utilities/utils.h \
    Presentation/guipresentation.h \
    DataAccess/db.h \
    dialogadd.h

FORMS    += mainwindow.ui \
    dialogadd.ui

QT += sql
