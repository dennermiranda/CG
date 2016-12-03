#include "closet.h"
#include "objreader.h"

Closet::Closet() {
    ObjReader reader;
    // For any object
    material = Material();
    reader.read("/Users/Pedro/Desktop/CG/Obj/closet.obj", material);

    faces = reader.getFaces();
    bottomY = reader.getBottomY();
}

Closet::Closet(Material m) {
    ObjReader reader;

    reader.read("/Users/Pedro/Desktop/CG/Obj/closet.obj", m);

    faces = reader.getFaces();
    bottomY = reader.getBottomY();
}
