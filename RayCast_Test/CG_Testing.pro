TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    material.cpp \
    sphere.cpp \
    object.cpp

HEADERS += \
    Camera.h \
    Color.h \
    Light.h \
    Object.h \
    Plane.h \
    Ray.h \
    Source.h \
    Sphere.h \
    Vect.h \
    material.h \
    primitive.h
