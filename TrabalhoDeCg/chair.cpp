#include "chair.h"
#include "objreader.h"

Chair::Chair() {
    ObjReader reader;
    // For any object
    material = Material();
    reader.read("/Users/Pedro/Desktop/CG/Obj/chair.obj", material);

    faces = reader.getFaces();
    bottomY = reader.getBottomY();
}

Chair::Chair(Material m) {
    ObjReader reader;

    reader.read("/Users/Pedro/Desktop/CG/Obj/chair.obj", m);

    faces = reader.getFaces();
    bottomY = reader.getBottomY();
}
