#include "lamp.h"
#include "objreader.h"

Lamp::Lamp() {
    ObjReader reader;
    // For any object
    material = Material();
    reader.read("/Users/Pedro/Desktop/CG/Obj/lamp.obj", material);

    faces = reader.getFaces();
    bottomY = reader.getBottomY();
}

Lamp::Lamp(Material m) {
    ObjReader reader;

    reader.read("/Users/Pedro/Desktop/CG/Obj/lamp.obj", m);

    faces = reader.getFaces();
    bottomY = reader.getBottomY();
}
