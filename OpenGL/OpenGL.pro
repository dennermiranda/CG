#-------------------------------------------------
#
# Project created by QtCreator 2016-12-21T17:13:19
#
#-------------------------------------------------

QT       += core gui opengl
LIBS = -lopengl32 -lglut32

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OpenGL
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    gldisplaywidget.cpp \
    vector.cpp \
    triangle.cpp \
    color.cpp \
    primitive.cpp \
    material.cpp

HEADERS  += mainwindow.h \
    gldisplaywidget.h \
    bed2.h \
    canto.h \
    chair.h \
    closet.h \
    desk.h \
    lamp.h \
    laptop.h \
    som.h \
    tv.h \
    vector.h \
    triangle.h \
    color.h \
    primitive.h \
    material.h

FORMS    += mainwindow.ui

DISTFILES +=
