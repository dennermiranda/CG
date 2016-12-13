#include "cenario.h"
#include "cubo.h"

Cenario::Cenario(Material color)
{

    //Piso
    Cubo piso(color);
    piso.scale(3, 0.1, 3);

    //Parede 1
    Cubo parede1(color);
    parede1.scale(3, 2, 0.1);
    parede1.translate(0, 1.9, -2.9);

    //Parede 2
    Cubo parede2(color);
    parede2.scale(0.1, 2, 3);
    parede2.translate(2.9, 1.9, 0);


    for(unsigned int i = 0; i < piso.getFaces().size(); i++) {
        faces.push_back(piso.getFaces().at(i));
    }
    for(unsigned int i = 0; i < parede1.getFaces().size(); i++) {
        faces.push_back(parede1.getFaces().at(i));
    }
    for(unsigned int i = 0; i < parede2.getFaces().size(); i++) {
        faces.push_back(parede2.getFaces().at(i));
    }

}
