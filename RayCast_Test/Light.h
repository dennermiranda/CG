#ifndef _Light_H
#define _Light_H

#include "Vect.h"
#include "Color.h"

class Light : public Source {
	Vect position;
    Color color_a;
    Color color_d;
    Color color_s;
	
	public:
	
	Light ();
	
    Light (Vect, Color, Color, Color);

	virtual Vect getLightPosition () { return position; }
    virtual Color getAmbientColor () { return color_a; }
    virtual Color getDifuseColor  () { return color_d;}
    virtual Color getSpecularColor  () { return color_s;}
	
};

Light::Light () {
    position = Vect(0,0,0); //Default position
    color_a = Color(1,1,1,0); //Default color (White)
    color_d = Color(1,1,1,0); //Default color (White)
    color_s = Color(1,1,1,0); //Default color (White)
}

Light::Light (Vect p, Color a, Color d, Color s) {
	position = p;
    color_a = a;
    color_d = d;
    color_s = s;
}

#endif // LIGHT_H
