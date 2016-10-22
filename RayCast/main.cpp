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

int winningObjectIndex(vector<double> object_intersections) {

    int index_of_minimum_value;

    //Preventing the program from unnecessary calculations

    if(object_intersections.size() == 0){ //so there's no intersections

        return -1;

    }else if (object_intersections.size() == 1){

        if (object_intersections.at(0) > 0){ //if that intersection is greater than zero then its our index of minimum value


            return 0; //index[0] is the object index

        }else{ //otherwise the only intersections value is negative

            return -1; //the ray missed everything

        }

    }else{ //there is more than one intersections

        double max = 0;

        for (int i = 0; i < object_intersections.size(); i++) {
            if (max < object_intersections.at(i)){
                max = object_intersections.at(i); //the pixels color will be the same as this obj
            }
        }

        //Then starting from the maximum value find the minimum position
        if (max > 0){//We only want positive intersections

            for (int index = 0; index < object_intersections.size(); index++) {
                if (object_intersections.at(index) > 0 && object_intersections.at(index) <= max){
                    max = object_intersections.at(index);
                    index_of_minimum_value = index;
                }
            }

            return index_of_minimum_value;

        }else{
            //all the intersections were negative
            return -1;
        }

    }
}

//Pixel position "thisone"
int px;

int main(int argc, char *argv[])
{
    cout << "Rendering..." << endl;

    int dpi = 72;
    int width = 640;
    int height = 480;

    int n = width*height;
    double aspectratio = (double)width/(double)height;

    RGBType *pixels = new RGBType[n];

    Vect O (0,0,0); //Origin vector
    Vect X (1,0,0);
    Vect Y (0,1,0); //ViewUp
    Vect Z (0,0,1);

//    Vect campos (3, 1.5, 4);
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

    //We need to index our scene objects
    vector<Object*> scene_objects;
    scene_objects.push_back(dynamic_cast<Object*>(&scene_sphere)); //Pushing the sphere into the vector
    scene_objects.push_back(dynamic_cast<Object*>(&scene_plane)); //Pushing the plane into the vector

    double xamnt, yamnt;

    for (int x = 0; x< width; x++){

        for (int y = 0; y< height; y++){
            //return colour
            px = y*width+x;

            //start with no anti-aliasing
            if (width > height) {
                //the image is wider than it is tall
                xamnt = ((x+0.5)/width)*aspectratio - (((width-height/(double)height/2)));
                yamnt = ((height - y)+ 0.5)/height;

            }else if (height > width) {
                //the image is taller than it is wide
                xamnt = ((x+0.5)/width);
                yamnt = (((height - y)+ 0.5)/height)/aspectratio - (((height - width)/(double)width)/2);

            }else {
                //the image is square
                xamnt = (x + 0.5)/width;
                yamnt = ((height - y) + 0.5/height);
            }

            //Creating rays
            Vect cam_ray_origin = scene_cam.getCameraPosition();
            Vect cam_ray_direction = camdir.vectAdd(camright.vectMult(xamnt - 0.5).vectAdd(camdown.vectMult(yamnt-0.5))).normalize();

            Ray cam_ray (cam_ray_origin, cam_ray_direction); //A specific camera ray is going through this specific xy pixel into the scene to look for intersections options

            //Creating an "array" of intersections since now we've got rays going through
            vector<double> intersections;

            //Checking if the ray which the loop is currently dealing with intersecs with any objects in the scene
            for (int index = 0 ; index < scene_objects.size() ; index++){
                intersections.push_back(scene_objects.at(index)->findIntersection(cam_ray));
            }

            int index_of_winning_object = winningObjectIndex(intersections); //Checking which object is the closest to the camera

            //cout << index_of_winning_object;

//            if((x>200 && x<440) && (y>200 && y < 280)){
//                pixels[px].r = 50;
//                pixels[px].g = 10;
//                pixels[px].b = 120;
//            }
//            else{
//                pixels[px].r = 0;
//                pixels[px].g = 0;
//                pixels[px].b = 0;
//            }

            if (index_of_winning_object == -1) { //setting the background color to black
                pixels[px].r = 0;
                pixels[px].g = 0;
                pixels[px].b = 0;

            }else{ //setting the color of the obj

                Color this_color = scene_objects.at(index_of_winning_object)->getColor();

                pixels[px].r = this_color.getColorRed();
                pixels[px].g = this_color.getColorGreen();
                pixels[px].b = this_color.getColorBlue();

            }


        }
    }

    saveBMP("scene.bmp", width, height, dpi, pixels);

    return 0;
}
