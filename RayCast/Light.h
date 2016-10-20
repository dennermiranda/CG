#ifndef LIGHT_H
#define LIGHT_H

#include "Vect.h"
#include "Color.h"

class Light {
    Vect position;
    Color color;

    public:

    Light();

    Light(Vect, Color);

    //Get functions

    Vect getLightPosition(){ return position; }
    Color getLightColor(){ return color; }

};

Light::Light (){
    position = Vect(0,0,0); //default position
    color = Color(1,1,1,0); //default color = white
}

Light::Light (Vect p, Color c){
    position = p;
    color = c;
}

#endif // LIGHT_H
