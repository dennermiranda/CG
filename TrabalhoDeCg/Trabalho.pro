#-------------------------------------------------
#
# Project created by QtCreator 2016-02-13T12:03:23
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Trabalho
TEMPLATE = app
#QMAKE_CXXFLAGS+= -openmp
#QMAKE_LFLAGS +=  -openmp
QMAKE_CXXFLAGS += -fopenmp
LIBS += -fopenmp


SOURCES += main.cpp\
        mainwindow.cpp \
    vector.cpp \
    ray.cpp \
    camera.cpp \
    color.cpp \
    light.cpp \
    sphere.cpp \
    object.cpp \
    plane.cpp \
    triangle.cpp \
    objreader.cpp \
    primitive.cpp \
    torus.cpp \
    material.cpp \
    cubo.cpp \
    wind.cpp \
    mesa.cpp \
    hex.cpp \
    bed.cpp \
    laptop.cpp \
    lamp.cpp \
    chair.cpp \
    tv.cpp \
    desk.cpp \
    closet.cpp

HEADERS  += mainwindow.h \
    vector.h \
    ray.h \
    camera.h \
    color.h \
    light.h \
    sphere.h \
    object.h \
    plane.h \
    triangle.h \
    objreader.h \
    primitive.h \
    torus.h \
    material.h \
    cubo.h \
    wind.h \
    mesa.h \
    hex.h \
    bed.h \
    laptop.h \
    lamp.h \
    chair.h \
    tv.h \
    desk.h \
    closet.h

FORMS    += mainwindow.ui
