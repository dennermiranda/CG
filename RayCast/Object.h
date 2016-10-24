#ifndef _OBJECT_H
#define _OBJECT_H

#include "Ray.h"
#include "Vect.h"
#include "Color.h"

class Object {

    public:

    Object ();

    virtual Color getColor () { return Color (0.0, 0.0, 0.0, 0); }

    virtual Vect getNormalAt(Vect intersection_position) {
        return Vect (0, 0, 0);
    }

    virtual double findIntersection(Ray ray) { //To return the intersection that ray makes with object
                return 0;
    }

};

Object::Object () {}

#endif // OBJECT_H
