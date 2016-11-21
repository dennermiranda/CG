#include "cubo.h"
#include "triangle.h"

Cubo::Cubo(Material color){

    //Vector de vértices:

    Vector A(+1,-1,+1); //v1
    Vector B(+1,-1,-1); //v2
    Vector C(-1,-1,-1); //v3
    Vector D(-1,-1,+1); //v4
    Vector E(+1,+1,+1); //v5
    Vector F(+1,+1,-1); //v6
    Vector G(-1,+1,-1); //v7
    Vector H(-1,+1,+1); //v8

    //Front side:
    faces.push_back(new Triangle(D, A, E, color));
    faces.push_back(new Triangle(E, H, D, color));

    //Right side:
    faces.push_back(new Triangle(A, B, F, color));
    faces.push_back(new Triangle(F, E, A, color));

    //Back side:
    faces.push_back(new Triangle(B, C, G, color));
    faces.push_back(new Triangle(G, F, B, color));

    //Left side:
    faces.push_back(new Triangle(C, D, H, color));
    faces.push_back(new Triangle(H, G, C, color));

    //Top:
    faces.push_back(new Triangle(D, A, B, color));
    faces.push_back(new Triangle(B, C, D, color));

    //Bottom:
    faces.push_back(new Triangle(H, E, F, color));
    faces.push_back(new Triangle(F, G, H, color));

}
