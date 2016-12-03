#include "laptop.h"
#include "objreader.h"

Laptop::Laptop() {
    ObjReader reader;
    // For any object
    material = Material();
    reader.read("/Users/Pedro/Desktop/CG/Obj/laptop.obj", material);

    faces = reader.getFaces();
    bottomY = reader.getBottomY();
}

Laptop::Laptop(Material m) {
    ObjReader reader;

    reader.read("/Users/Pedro/Desktop/CG/Obj/laptop.obj", m);

    faces = reader.getFaces();
    bottomY = reader.getBottomY();
}
