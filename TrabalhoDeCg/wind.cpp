#include "wind.h"
#include "objreader.h"

Wind::Wind() {
    ObjReader reader;
    // For any object
    material = Material();
    reader.read("/Users/Pedro/Desktop/CG/TrabalhoDeCg/hex.obj", material);
//    reader.read("hex.obj", material); //Dener

    faces = reader.getFaces();
    bottomY = reader.getBottomY();
}

Wind::Wind(Material m) {
    ObjReader reader;

    reader.read("/Users/Pedro/Desktop/CG/TrabalhoDeCg/Windmill.obj", m);
//    reader.read("Windmill.obj", m);//Dener

    faces = reader.getFaces();
    bottomY = reader.getBottomY();
}
