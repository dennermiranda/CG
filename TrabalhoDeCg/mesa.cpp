#include "mesa.h"
#include "cubo.h"
Mesa::Mesa(Material color)
{

    //Tampo
    Cubo tampo(color);
    //tampo.scale(1.1, 0.15, 0.6);
    //tampo.scale(2.2, 0.15, 1.2);


    //Perna1
    Cubo perna1(color);
    perna1.scale(0.1, 0.72, 0.06);
    perna1.translate(2.1, -0.85, 1.14);


    //Perna 2
    Cubo perna2(color);
    perna2.scale(0.1, 0.72, 0.06);
    perna2.translate(-2.1, -0.85, 1.14);


    //Perna 3
    Cubo perna3(color);
    perna3.scale(0.1, 0.72, 0.06);
    perna3.translate(-2.1, -0.85, -1.14);


    //Perna 4
    Cubo perna4(color);
    perna4.scale(0.1, 0.72, 0.06);
    perna4.translate(2.1, -0.85, -1.14);

    for(unsigned int i = 0; i < tampo.getFaces().size(); i++) {
        faces.push_back(tampo.getFaces().at(i));
    }
    /*for(unsigned int i = 0; i < perna1.getFaces().size(); i++) {
        faces.push_back(perna1.getFaces().at(i));
    }
    for(unsigned int i = 0; i < perna2.getFaces().size(); i++) {
        faces.push_back(perna2.getFaces().at(i));
    }
    for(unsigned int i = 0; i < perna3.getFaces().size(); i++) {
        faces.push_back(perna3.getFaces().at(i));
    }
    for(unsigned int i = 0; i < perna4.getFaces().size(); i++) {
        faces.push_back(perna4.getFaces().at(i));
    }*/

}
