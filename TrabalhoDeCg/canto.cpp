#include "canto.h"
#include "objreader.h"

Canto::Canto() {
    ObjReader reader;
    // For any object
    material = Material();
    reader.read("/Users/Pedro/Desktop/CG/Obj/canto.obj", material);

    faces = reader.getFaces();
    bottomY = reader.getBottomY();
}

Canto::Canto(Material m) {
    ObjReader reader;

    reader.read("/Users/Pedro/Desktop/CG/Obj/canto.obj", m);

    faces = reader.getFaces();
    bottomY = reader.getBottomY();
}
