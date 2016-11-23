#include "hex.h"
#include "objreader.h"

Hex::Hex() {
    ObjReader reader;
    // For any object
    material = Material();
    reader.read("/Users/Pedro/Desktop/CG/TrabalhoDeCg/hex.obj", material);
//    reader.read("hex.obj", material); //Dener

    faces = reader.getFaces();
    bottomY = reader.getBottomY();
}

Hex::Hex(Material m) {
    ObjReader reader;

    reader.read("/Users/Pedro/Desktop/CG/TrabalhoDeCg/hex.obj", m);
//    reader.read("hex.obj", m);//Dener

    faces = reader.getFaces();
    bottomY = reader.getBottomY();
}
