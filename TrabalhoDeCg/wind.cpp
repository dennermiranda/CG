#include "wind.h"
#include "objreader.h"

Wind::Wind() {
    ObjReader reader;
    // For any object
    material = Material();
    reader.read("hex.obj", material);

    faces = reader.getFaces();
    bottomY = reader.getBottomY();
}

Wind::Wind(Material m) {
    ObjReader reader;

    reader.read("/Users/Pedro/Desktop/CG/TrabalhoDeCg/Windmill.obj", m);

    faces = reader.getFaces();
    bottomY = reader.getBottomY();
}
