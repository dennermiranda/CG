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
#include "Source.h"
#include "Light.h"
#include "Object.h"
#include "Sphere.h"
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

    //Preventing the program from unnecessary calculations:

    if (object_intersections.size() == 0) { //There're no intersections
        return -1;
    }
    else if (object_intersections.size() == 1) {

        if (object_intersections.at(0) > 0) { //If that intersection is greater than zero then its our index of minimum value

            return 0; //index[0] is the object index

        }else{ //Otherwise the only intersection value is negative
            return -1;
        }

    }else{ //Otherwise there is more than one intersection - So first we need to find the maximum value

        double max = 0;
        for (int i = 0; i < object_intersections.size(); i++) {
            if (max < object_intersections.at(i)) {
                max = object_intersections.at(i); //The pixel's color will be the same as this object's
            }
        }

        //Then starting from the maximum value we gotta find the minimum position

        if (max > 0) { //We only want positive intersections

            for (int index = 0; index < object_intersections.size(); index++) {

                if (object_intersections.at(index) > 0 && object_intersections.at(index) <= max) {
                    max = object_intersections.at(index);
                    index_of_minimum_value = index;

                }
            }

            return index_of_minimum_value;

        }else { //All the intersections were negative
            return -1;
        }
    }
}

Color getColorAt(Vect intersection_position, Vect intersecting_ray_direction, vector<Object*> scene_objects, int index_of_winning_object, vector<Source*> light_sources, double accuracy, double ambientLight) {

    Color winning_object_color = scene_objects.at(index_of_winning_object)->getColor();
    Vect winning_object_normal = scene_objects.at(index_of_winning_object)->getNormalAt(intersection_position);

    Color final_color = winning_object_color.colorScalar(ambientLight);

    for (int light_index = 0; light_index < light_sources.size(); light_index++) {

        Vect light_direction = light_sources.at(light_index)->getLightPosition().vectAdd(intersection_position.negative()).normalize();

        float cosine_angle = winning_object_normal.dotProduct(light_direction); //Angle between these vectors

        if (cosine_angle > 0) { //Test for shadows

            bool shadowed = false;  //Shadow by default or not

            Vect distance_to_light = light_sources.at(light_index)->getLightPosition().vectAdd(intersection_position.negative()).normalize();
            float distance_to_light_magnitude = distance_to_light.magnitude();

            Ray shadow_ray (intersection_position, light_sources.at(light_index)->getLightPosition().vectAdd(intersection_position.negative()).normalize());

            vector<double> secondary_intersections;

            for (int object_index = 0; object_index < scene_objects.size() && shadowed == false; object_index++) {
                secondary_intersections.push_back(scene_objects.at(object_index)->findIntersection(shadow_ray));
            }

            for (int c = 0; c < secondary_intersections.size(); c++) {
                if (secondary_intersections.at(c) > accuracy) {
                    if (secondary_intersections.at(c) <= distance_to_light_magnitude) {
                        shadowed = true;
                    }
                    break;
                }
            }

            if (shadowed == false) {

                final_color = final_color.colorAdd(winning_object_color.colorMultiply(light_sources.at(light_index)->getLightColor()).colorScalar(cosine_angle));

                if (winning_object_color.getColorSpecial() > 0 && winning_object_color.getColorSpecial() <= 1) {

                    //Special [0-1]
                    double dot1 = winning_object_normal.dotProduct(intersecting_ray_direction.negative());
                    Vect scalar1 = winning_object_normal.vectMult(dot1);
                    Vect add1 = scalar1.vectAdd(intersecting_ray_direction);
                    Vect scalar2 = add1.vectMult(2);
                    Vect add2 = intersecting_ray_direction.negative().vectAdd(scalar2);
                    Vect reflection_direction = add2.normalize(); //Determining the reflexion direction

                    double specular = reflection_direction.dotProduct(light_direction);

                    if (specular > 0) {
                        specular = pow(specular, 10);
                        final_color = final_color.colorAdd(light_sources.at(light_index)->getLightColor().colorScalar(specular*winning_object_color.getColorSpecial()));
                    }

                }
            }

        }

    }

//    return Color (0, 0, 0, 0);
    return final_color.clip();

}

int px;

int main (int argc, char *argv[]) {

    cout << "Rendering ..." << endl;

    int dpi = 72;
    int width = 640;
    int height = 480;

    int n = width*height;

    double aspectratio = (double)width/(double)height;
    double ambientLight = 0.2;
    double accuracy = 0.000001;

    RGBType *pixels = new RGBType[n];


    Vect O (0,0,0); //Origin vector
    Vect X (1,0,0);
    Vect Y (0,1,0); //ViewUp
    Vect Z (0,0,1);

    Vect CamPosition (3, 1.5, -4);

    Vect look_at (0, 0, 0);
    Vect diff_btw (CamPosition.getVectX() - look_at.getVectX(), CamPosition.getVectY() - look_at.getVectY(), CamPosition.getVectZ() - look_at.getVectZ());

    Vect camDirection = diff_btw.negative().normalize(); //Cameras Direction normalized (k)
    Vect camRight = Y.crossProduct(camDirection).normalize(); //Cameras Right normalized (i)
    Vect camDown = camRight.crossProduct(camDirection); //Cameras down (j)

    Camera scene_cam (CamPosition, camDirection, camRight, camDown);

    //Creating our colors:
    Color white_light (1.0, 1.0, 1.0, 0);
    Color pretty_green (0.5, 1.0, 0.5, 0.3);
    Color maroon (0.5, 0.25, 0.25, 0);
    Color tile_floor (1, 1, 1, 2);
    Color gray (0.5, 0.5, 0.5, 0);
    Color black (0.0, 0.0, 0.0, 0);


    //Creating our light source:
    Vect light_position (-7,10,-10);
    Light scene_light (light_position, white_light);
    vector<Source*> light_sources;
    light_sources.push_back(dynamic_cast<Source*>(&scene_light));

    //Creating scene's objects:
    Sphere scene_sphere (O, 1, pretty_green); //(Position, Radius, Color);
    Plane scene_plane (Y, -1, maroon); //(Up and down direction of the scene, Distance from the center of our plane to the center of the scene (right beneath the sphere), Color)

    //Storing scene's objects:
    vector<Object*> scene_objects;
    scene_objects.push_back(dynamic_cast<Object*>(&scene_sphere)); //Pushing the sphere into the vector
    scene_objects.push_back(dynamic_cast<Object*>(&scene_plane)); //Pushing the plane into the vector

    double xamnt, yamnt;

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {

            px = y*width + x;

            //Start with no anti-aliasing

            if (width > height) { // The image is wider than it is tall

                xamnt = ((x+0.5)/width)*aspectratio - (((width-height)/(double)height)/2);
                yamnt = ((height - y) + 0.5)/height;

            }else if (height > width) { // The imager is taller than it is wide

                xamnt = (x + 0.5)/ width;
                yamnt = (((height - y) + 0.5)/height)/aspectratio - (((height - width)/(double)width)/2);

            }else{ // The image is square

                xamnt = (x + 0.5)/width;
                yamnt = ((height - y) + 0.5)/height;

            }

            //Creating rays:
            Vect cam_ray_origin = scene_cam.getCameraPosition();
            Vect cam_ray_direction = camDirection.vectAdd(camRight.vectMult(xamnt - 0.5).vectAdd(camDown.vectMult(yamnt - 0.5))).normalize();

            Ray cam_ray (cam_ray_origin, cam_ray_direction); //A specific camera ray is going through this specific xy pixel into the scene to look for intersections options

            //Creating an "array" of intersections since now we've got rays going through:
            vector<double> intersections;

            //Checking if the ray which the loop is currently dealing with intersecs with any objects in the scene
            for (int index = 0; index < scene_objects.size(); index++) {

                intersections.push_back(scene_objects.at(index)->findIntersection(cam_ray));

            }

            int index_of_winning_object = winningObjectIndex(intersections); //Getting the nearest object

//            cout << index_of_winning_object; //Testing...

            if (index_of_winning_object == -1) {

                //Setting the backgroung color to black:

                pixels[px].r = 0;
                pixels[px].g = 0;
                pixels[px].b = 0;

            }else{

                //Setting the objects color:

                if (intersections.at(index_of_winning_object) > accuracy) { //Determine the position and direction vectors at the point of intersection

                    Vect intersection_position = cam_ray_origin.vectAdd(cam_ray_direction.vectMult(intersections.at(index_of_winning_object)));
                    Vect intersecting_ray_direction = cam_ray_direction; // The direction of the intersecting ray is the direction of the ray is coming from which is the camera

//                    Color this_color = scene_objects.at(index_of_winning_object)->getColor();

                    Color intersection_color = getColorAt(intersection_position, intersecting_ray_direction, scene_objects, index_of_winning_object, light_sources, accuracy, ambientLight); //Getting the color at the intersection

                    pixels[px].r = intersection_color.getColorRed();
                    pixels[px].g = intersection_color.getColorGreen();
                    pixels[px].b = intersection_color.getColorBlue();

//                    pixels[px].r = this_color.getColorRed();
//                    pixels[px].g = this_color.getColorGreen();
//                    pixels[px].b = this_color.getColorBlue();
                }

            }

        }

    }

    saveBMP("scene.bmp", width, height, dpi, pixels);

    return 0;

}
