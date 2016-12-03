#include "desk.h"
#include "objreader.h"

Desk::Desk() {
    ObjReader reader;
    // For any object
    material = Material();
    reader.read("/Users/Pedro/Desktop/CG/Obj/desk.obj", material);

    faces = reader.getFaces();
    bottomY = reader.getBottomY();
}

Desk::Desk(Material m) {
    ObjReader reader;

    reader.read("/Users/Pedro/Desktop/CG/Obj/desk.obj", m);

    faces = reader.getFaces();
    bottomY = reader.getBottomY();
}
