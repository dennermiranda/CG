#ifndef _VECT_H
#define _VECT_H

#include <cmath>
#include "math.h"

class Vect {
	double x, y, z;
	
	public:
	
	Vect ();
	
	Vect (double, double, double);

	double getVectX() { return x; }
	double getVectY() { return y; }
	double getVectZ() { return z; }
	
	double magnitude () {
		return sqrt((x*x) + (y*y) + (z*z));
	}
	
	Vect normalize () {
		double magnitude = sqrt((x*x) + (y*y) + (z*z));
		return Vect (x/magnitude, y/magnitude, z/magnitude);
	}
	
	Vect negative () {
		return Vect (-x, -y, -z);
	}
	
	double dotProduct(Vect v) {
		return x*v.getVectX() + y*v.getVectY() + z*v.getVectZ();
	}
	
	Vect crossProduct(Vect v) {
		return Vect (y*v.getVectZ() - z*v.getVectY(), z*v.getVectX() - x*v.getVectZ(), x*v.getVectY() - y*v.getVectX());
	}
	
	Vect vectAdd (Vect v) {
		return Vect (x + v.getVectX(), y + v.getVectY(), z + v.getVectZ());
	}

	Vect vectMult (double scalar) {
		return Vect (x*scalar, y*scalar, z*scalar);
	}

    void translate (double tx, double ty, double tz){
        x += tx;
        y += ty;
        z += tz;
    }

    void scale (double sx, double sy, double sz){
        x *= sx;
        y *= sy;
        z *= sz;
    }

    void rotateX(double angle){
        double newY = y*cos(angle) - z*sin(angle);
        double newZ = y*sin(angle) + z*cos(angle);

        y = newY;
        z = newZ;
    }

    void rotateY(double angle){
        double newX = x*cos(angle) + z*sin(angle);
        double newZ = (-x)*sin(angle) + z*cos(angle);

        x = newX;
        z = newZ;
    }

    void rotateZ(double angle){
        double newX = x*cos(angle) - y*sin(angle);
        double newY = x*sin(angle) + y*cos(angle);

        x = newX;
        y = newY;
    }
};

Vect::Vect () {
	x = 0;
	y = 0;
	z = 0;
}

Vect::Vect (double i, double j, double k) {
	x = i;
	y = j;
	z = k;
}

#endif
