#ifndef _Plane_H
#define _Plane_H

#include "math.h"
#include "primitive.h"
#include "Vect.h"
#include "material.h"

class Plane : public Primitive {

    Vect normal; //Normal vector perpendicular to the surface

    double distance; //Distance from the center of the plane from the center of the scene (Distance from the origin)

    //Material color;
	
	public:
	
	Plane ();
	
    Plane (Vect, double, Material);
	
	// method functions
	Vect getPlaneNormal () { return normal; }
	double getPlaneDistance () { return distance; }
    virtual Material getColor () { return color; }
	
    //Calculating the reflexions:

	virtual Vect getNormalAt(Vect point) {
		return normal;
	}

    void translate (double tx, double ty, double tz){

    }
    void scale (double sx, double sy, double sz){

    }

    void rotateX(double angle) {

    }

    void rotateY(double angle) {

    }

    void rotateZ(double angle) {

    }

	virtual double findIntersection(Ray ray) {

        Vect ray_direction = ray.getRayDirection();
		
        //Keeping in mind that the ray (from the camera) will terminate at the intersection point if the ray intersects something

		double a = ray_direction.dotProduct(normal);
		
        if (a == 0) { //Ray is parallel to the plane
			return -1;

        }else{

			double b = normal.dotProduct(ray.getRayOrigin().vectAdd(normal.vectMult(distance).negative()));

            return -1*b/a; //Negative because is in the oposite direction of the origin from the ray

        }
	}


	
};

Plane::Plane () {
    normal = Vect(1,0,0); //Default orientation - x is up
	distance = 0;
    material = Material();
}

Plane::Plane (Vect normalValue, double distanceValue, Material colorValue) {
	normal = normalValue;
	distance = distanceValue;
    //color = colorValue;
    this->material = colorValue;
}

#endif
