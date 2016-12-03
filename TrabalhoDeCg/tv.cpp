#include "tv.h"
#include "objreader.h"

Tv::Tv() {
    ObjReader reader;
    // For any object
    material = Material();
    reader.read("/Users/Pedro/Desktop/CG/Obj/tv.obj", material);

    faces = reader.getFaces();
    bottomY = reader.getBottomY();
}

Tv::Tv(Material m) {
    ObjReader reader;

    reader.read("/Users/Pedro/Desktop/CG/Obj/tv.obj", m);

    faces = reader.getFaces();
    bottomY = reader.getBottomY();
}
