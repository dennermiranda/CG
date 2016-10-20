#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <vector>
#include <cmath>
#include <limits>

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "Vect.h"
#include "Ray.h"
#include "Camera.h"
#include "Color.h"
#include "Light.h"
#include "Sphere.h"
#include "Object.h"
#include "Plane.h"

using namespace std;

struct RGBType {
    double r;
    double g;
    double b;
};

void saveBMP(const std::string filename, int w, int h, int dpi, RGBType* data){
    FILE* file;
    int totalSize = w * h;
    int stride = 4 * totalSize;
    int fileSize = 54 + stride;

    double factor = 39.375;
    int meter = static_cast<int>(factor);
    int pixelsPerMeter = dpi * meter;

    unsigned char bmpFileHeader[14] = {'B', 'M', 0,0,0,0, 0,0,0,0, static_cast<char>(54),0,0,0 };
    unsigned char bmpInfoHeader[40];

    std::memset(bmpInfoHeader, 0, sizeof(bmpInfoHeader));

    bmpInfoHeader[0] = static_cast<char>(40);
    bmpInfoHeader[12] = 1;
    bmpInfoHeader[14] = 24;

    //Header: Signature, Filesize, Reserved1, Reserved2, File Offset to Pixel Array
    bmpFileHeader[2] = (unsigned char)(fileSize);
    bmpFileHeader[3] = (unsigned char)(fileSize >> 8);
    bmpFileHeader[4] = (unsigned char)(fileSize >> 16);
    bmpFileHeader[5] = (unsigned char)(fileSize >> 24);

    //Width of the bitmap
    bmpInfoHeader[4] = (unsigned char)(w);
    bmpInfoHeader[5] = (unsigned char)(w>>8);
    bmpInfoHeader[6] = (unsigned char)(w>>16);
    bmpInfoHeader[7] = (unsigned char)(w>>24);

    //Height of the bitmap
    bmpInfoHeader[8] = (unsigned char)(h);
    bmpInfoHeader[9] = (unsigned char)(h>>8);
    bmpInfoHeader[10] = (unsigned char)(h>>16);
    bmpInfoHeader[11] = (unsigned char)(h>>24);

    //Pixels per meter Density
    bmpInfoHeader[21] = (unsigned char)(pixelsPerMeter);
    bmpInfoHeader[22] = (unsigned char)(pixelsPerMeter>>8);
    bmpInfoHeader[23] = (unsigned char)(pixelsPerMeter>>16);
    bmpInfoHeader[24] = (unsigned char)(pixelsPerMeter>>24);

    bmpInfoHeader[25] = (unsigned char)(pixelsPerMeter);
    bmpInfoHeader[26] = (unsigned char)(pixelsPerMeter>>8);
    bmpInfoHeader[27] = (unsigned char)(pixelsPerMeter>>16);
    bmpInfoHeader[28] = (unsigned char)(pixelsPerMeter>>24);

    bmpInfoHeader[29] = (unsigned char)(pixelsPerMeter);
    bmpInfoHeader[30] = (unsigned char)(pixelsPerMeter>>8);
    bmpInfoHeader[31] = (unsigned char)(pixelsPerMeter>>16);
    bmpInfoHeader[32] = (unsigned char)(pixelsPerMeter>>24);

    file = std::fopen(filename.c_str(), "wb"); //wb := 'W'rite 'B'inary

    std::fwrite(bmpFileHeader, 1, 14, file);
    std::fwrite(bmpInfoHeader, 1, 40, file);

    for (int i =0; i<totalSize; i++){
        RGBType rgb = data[i];

        double red = (data[i].r)*255;
        double green = (data[i].g)*255;
        double blue = (data[i].b)*255;

        unsigned char color[3] = {
            static_cast<unsigned char>((int) std::floor(blue)),
            static_cast<unsigned char>((int) std::floor(green)),
            static_cast<unsigned char>((int) std::floor(red))
        };

        std::fwrite(color, 1, 3, file);
    }

    std::fclose(file);
}
//Pixel position "thisone"
int px;
int main(int argc, char *argv[])
{
    cout << "Rendering" << endl;

    int dpi = 72;
    int width = 640;
    int height = 480;

    int n = width*height;

    RGBType *pixels = new RGBType[n];

    Vect O (0,0,0); //Origin vector
    Vect X (1,0,0);
    Vect Y (0,1,0);
    Vect Z (0,0,1);

    Vect campos (3, 1.5, -4);

    Vect look_at (0,0,0);

    Vect diff_btw (campos.getVectX() - look_at.getVectX(), campos.getVectY() - look_at.getVectY(), campos.getVectZ() - look_at.getVectZ()); // difference

    Vect camdir = diff_btw.negative().normalize(); //camera direction normalized //k?
    Vect camright = Y.crossProduct(camdir).normalize(); //camera right normalized //i?
    Vect camdown = camright.crossProduct(camdir); //j?

    Camera scene_cam(campos, camdir, camright, camdown);

    //Creating our colors

    Color white_light (1.0, 1.0, 1.0, 0);
    Color pretty_green (0.5, 1.0, 0.5, 0.3);
    Color maroon (0.5, 0.25, 0.25, 0);
    Color gray (0.5, 0.5, 0.5, 0);
    Color black(0.0, 0.0, 0.0, 0);


    //Creating our light source

    Vect light_position (-7,10,-10);
    Light scene_light (light_position, white_light);

    //Creating scene objects

    Sphere scene_sphere (O, 1, pretty_green); //(position, radius, color);
    Plane scene_plane (Y,  -1, maroon); //(up and down direction of the scene, distance from the center of our plane to the center of the scene (right beneath the sphere), color)

    for (int x = 0; x< width; x++){

        for (int y = 0; y< height; y++){
            //return colour
            px = y*width+x;

            if((x>200 && x<440) && (y>200 && y < 280)){
                pixels[px].r = 50;
                pixels[px].g = 10;
                pixels[px].b = 120;
            }
            else{
                pixels[px].r = 0;
                pixels[px].g = 0;
                pixels[px].b = 0;
            }

        }
    }

    saveBMP("scene.bmp", width, height, dpi, pixels);

    return 0;
}
