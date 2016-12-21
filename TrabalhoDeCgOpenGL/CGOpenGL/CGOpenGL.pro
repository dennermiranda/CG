QT       += core gui

CONFIG += c++11

TARGET = CGOpenGL
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    window.cpp

HEADERS += \
    window.h \
    bed2.h \
    canto.h \
    chair.h \
    closet.h \
    desk.h \
    lamp.h \
    laptop.h \
    som.h \
    tv.h
