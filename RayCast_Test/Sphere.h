#ifndef SPHERE_H
#define SPHERE_H

#include <cmath>
#include "primitive.h"
#include "Vect.h"
#include "Color.h"

class Sphere : public Primitive {
    Vect center;
    double radius;
    Material color;

public:
    Sphere();
    Sphere(Vect, double, Material);

    Vector getCenter() { return center; }
    double rad() { return radius; }
    virtual Material getColor() { return color; }

    virtual Vect getNormalAt(Vector p);
    virtual double findIntersection(Ray ray);

    virtual void translate(double tx, double ty, double tz);
    virtual void scale(double sx, double sy, double sz);
    virtual void rotateX(double angle);
    virtual void rotateY(double angle);
    virtual void rotateZ(double angle);
};

#endif // SPHERE_H
