#ifndef PLANE_H
#define PLANE_H

#include "math.h"
#include "Object.h"
#include "Vect.h"
#include "Color.h"

class Plane : public Object {
    Vect normal; //normal vector perpendicular to the surface
    double distance; //distance from the origin - distance from the center of the plane from the center of the scene
    Color color;

    public:

    Plane();

    Plane(Vect, double, Color);

    //Get functions

    Vect getPlaneNormal(){ return normal; }
    double getPlaneDistance(){ return distance; }
    virtual Color getColor(){ return color; }

    //Calculating the reflexions

    Vect getNormalAt(Vect point) {
        //the normal at a particular point on the sphere will be the normal of this whole plane
        return normal;
    }

    virtual double findIntersection(Ray ray) { //double means the distance from the ray origin to the point of intersection
        Vect ray_direction = ray.getRayDirection();

        //Remembering that the ray (from the camera) will terminate at the intersection point if the ray intersects something
        double a = ray_direction.dotProduct(normal); //dot product between the ray and the normal

        if  (a == 0 ){ //it means the the ray is parallel to the plane so it nevers intersects
            return -1;

        }else{//Finding the intersection between rays and the plane

            double b = normal.dotProduct(ray.getRayOrigin().vectAdd(normal.vectMult(distance).negative()));
            //dot product between the normal vector and another vector witch is the result of ...????
            //negative because is in the oposite direction of the origin from the ray
            return -1*b/a;
        }

    }

};

Plane::Plane (){
    normal = Vect(1,0,0); //default orientation - x is up
    distance =  0;
    color = Color(0.5,0.5,0.5,0); //default color = grey
}

Plane::Plane (Vect normalValue, double distanceValue, Color colorValue){
    normal = normalValue;
    distance = distanceValue;
    color = colorValue;
}

#endif // PLANE_H
