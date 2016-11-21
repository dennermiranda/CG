#include "torus.h"
#include "objreader.h"

Torus::Torus() {
    ObjReader reader;

    material = Material();
    reader.read("torus.obj", material);

    faces = reader.getFaces();
    bottomY = reader.getBottomY();
}

Torus::Torus(Material m) {
    ObjReader reader;

    reader.read("torus.obj", m);

    faces = reader.getFaces();
    bottomY = reader.getBottomY();
}
