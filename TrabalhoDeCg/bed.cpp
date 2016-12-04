#include "bed.h"
#include "objreader.h"

Bed::Bed() {
    ObjReader reader;

    material = Material();
//    reader.read("/Users/Pedro/Desktop/CG/TrabalhoDeCg/bed2.obj", material);
    reader.read("/Users/Pedro/Desktop/CG/Obj/bed2.obj", material);

    faces = reader.getFaces();
    bottomY = reader.getBottomY();
}

Bed::Bed(Material m) {
    ObjReader reader;

    reader.read("/Users/Pedro/Desktop/CG/Obj/bed2.obj", m);
//    reader.read("bed.obj", m);

    faces = reader.getFaces();
    bottomY = reader.getBottomY();
}
