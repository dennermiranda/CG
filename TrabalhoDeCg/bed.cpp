#include "bed.h"
#include "objreader.h"

Bed::Bed() {
    ObjReader reader;

    material = Material();
//    reader.read("/Users/Pedro/Desktop/CG/TrabalhoDeCg/bed.obj", material);
    reader.read("/Users/Pedro/Desktop/CG/Obj/bed.obj", material);

    faces = reader.getFaces();
    bottomY = reader.getBottomY();
}

Bed::Bed(Material m) {
    ObjReader reader;

//    reader.read("/Users/Pedro/Desktop/CG/TrabalhoDeCg/bed.obj", m);
    reader.read("/Users/Pedro/Desktop/CG/Obj/bed.obj", m);

    faces = reader.getFaces();
    bottomY = reader.getBottomY();
}
