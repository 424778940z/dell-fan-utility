#-------------------------------------------------
#
# Project created by QtCreator 2015-09-01T16:34:35
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = dell_fan_utility
TEMPLATE = app

#QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    bzh_dell_smm_io_drv_opt.cpp \
    bzh_dell_smm_io_drv_fun.cpp \
    bzh_dell_smm_io_fan_opt.cpp

HEADERS  += mainwindow.h \
    bzh_dell_smm_io_drv_opt.h \
    bzh_dell_smm_io_drv_fun.h \
    bzh_dell_smm_io_codes.h \
    bzh_dell_smm_io_fan_opt.h \
    bzh_dell_smm_io_includes.h \
    bzh_dell_fan_utility_ui.h \
    bzh_dell_fan_utility_th.h \
    bzh_dell_fan_utility_drv.h


FORMS    += mainwindow.ui
