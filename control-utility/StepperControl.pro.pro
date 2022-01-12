#-------------------------------------------------
#
# Project created by QtCreator 2018-02-17T13:00:09
#
#-------------------------------------------------

QT       += core gui
QT += widgets serialport

TARGET = StepperControl

TEMPLATE = app

SOURCES *= \
        main.cpp \
        mainwindow.cpp \
        settingsdialog.cpp \
    stepper.cpp

HEADERS *= \
    settingsdialog.h \
    mainwindow.h \
    settingsdialog.h \
    stepper.h

FORMS *= \
        mainwindow.ui \
        settingsdialog.ui \
    stepper.ui

RESOURCES *= \
    resources.qrc
