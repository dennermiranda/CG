#ifndef OBJECT_H
#define OBJECT_H

#include "Ray.h"
#include "Vect.h"
#include "Color.h"

class Object {
    public:

    Object();

    //Get functions

    Color getObjectColor(){ return Color (0.0, 0.0, 0.0, 0); }

    double findIntersection (Ray ray) { //To return the intersection that ray makes with object
        return 0;
    }
};

Object::Object (){}

#endif // OBJECT_H
