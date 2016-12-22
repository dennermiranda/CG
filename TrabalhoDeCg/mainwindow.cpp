#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <string>
#include <limits>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <ctime>

#include "primitive.h"
#include "vector.h"
#include "ray.h"
#include "camera.h"
#include "material.h"
#include "color.h"
#include "light.h"
#include "object.h"
#include "sphere.h"
#include "plane.h"
#include "triangle.h"
#include "torus.h"
#include "cubo.h"
#include "wind.h"
#include "mesa.h"
#include "hex.h"
#include "bed.h"
#include "closet.h"
#include "lamp.h"
#include "laptop.h"
#include "chair.h"
#include "desk.h"
#include "tv.h"
#include "cenario.h"
#include "canto.h"


using namespace std;

/* Globals */
vector<Primitive *> objects;
vector<Light *> light_sources;

//double accuracy = 0.0000000001;

int W = 320*2;
int H = 240*2;

Color white(1.0, 1.0, 1.0);
Color black(0, 0, 0);

Material green(Color(0.2, 0.2, 0.2), Color(0.5, 1.0, 0.5), Color(0.5, 1.0, 0.5), 2);
Material brown(Color(0.2, 0.2, 0.2), Color(0.4, 0.2, 0.25), Color(0.1, 0.1, 0.1), 0);
Material metal(Color(0.2, 0.2, 0.2), Color(0.1, 0.1, 0.1), Color(1, 1, 1), 50);
Material red(Color(0.2, 0.2, 0.2), Color(1,0.1,0.1), Color(1, 1, 1), 70);
Material brass(Color(0.33, 0.22, 0.03), Color(0.78, 0.57, 0.11), Color(0.99, 0.91, 0.81), 27.8);
Material piso1(Color(0.8, 0.8, 0.8), Color(0.8, 0.8, 0.8), Color(0.8, 0.8, 0.8), 1);
Material blacktv(Color(0, 0, 0), Color(0, 0, 0), Color(0, 0, 0), 1);
Material realRed(Color(0.7, 0.0, 0.0), Color(0.7, 0.0, 0.0), Color(1, 1, 1), 70);
Material realYellow(Color(0.6, 0.8, 0.0), Color(0.6, 0.8, 0.0), Color(1, 1, 1), 70);
Material realBrown(Color(0.7, 0.5, 0.3), Color(0.7, 0.5, 0.3), Color(0.7, 0.5, 0.3), 70);


//Materiais & texturas:

Material gold(Color(0.25, 0.20, 0.07), Color(0.75, 0.61, 0.23), Color(0.63, 0.65, 0.37), 70); //0.2
Material emerald(Color(0.0215, 0.1745, 0.0215), Color(0.07568, 0.61424, 0.07568), Color(0.633, 0.727811, 0.633), 70); //0.6
Material jade(Color(0.135, 0.2225, 0.1575), Color(0.54, 0.89, 0.63), Color(0.31622, 0.31622, 0.31622), 70); //0.1
Material silver(Color(0.19, 0.19, 0.19), Color(0.51, 0.51, 0.51), Color(0.51, 0.51, 0.51), 70); //0.4
Material ruby(Color(0.1745, 0.01175, 0.01175), Color(0.61424, 0.04136, 0.04136), Color(0.727811, 0.626959, 0.626959), 0.6); //0.6
Material copper(Color(0.19, 0.07, 0.02), Color(0.7, 0.27, 0.08), Color(0.26, 0.14, 0.09), 70); //0.1

//Plastico:

Material blackPlastic(Color(0.0, 0.0, 0.0), Color(0.01, 0.01, 0.01), Color(0.5, 0.5, 0.5), 70); //0.25
Material cyanPlastic(Color(0, 0.1, 0.06), Color(0, 0.509803, 0.509803), Color(0.50196078, 0.50196078, 0.50196078), 0.25); //0.25
Material greenPlastic(Color(0, 0, 0), Color(0.1, 0.35, 0.1), Color(0.45, 0.55, 0.45), 70); //0.25
Material yellowPlastic(Color(0.0, 0.0, 0.0), Color(0.5, 0.5, 0.0), Color(0.6, 0.6, 0.5), 70); //0.25

//Borracha:

Material greenRubber(Color(0.05, 0.05, 0.0), Color(0.5, 0.5, 0.4), Color(0.7, 0.7, 0.04), 0.078125); //0.078125
Material whiteRubber(Color(0.05, 0.05, 0.0), Color(0.5, 0.5, 0.5), Color(0.7, 0.7, 0.7), 0.078125); //0.078125
Material yellowRubber(Color(0.05, 0.05, 0.0), Color(0.5, 0.5, 0.4), Color(0.7, 0.7, 0.04), 70); //0.078125


Vector X(1, 0, 0);
Vector Y(0, 1, 0);
Vector Z(0, 0, 1);
Vector O(0, 0, 0);

long  getIndexOfClosestObject(vector<double>  intersections) {

    if (intersections.size() == 0) //return null if intersections vector is empty
        return -1;

    if (intersections.size() == 1) { //theres just one
        if (intersections.at(0) > 0)
            return 0;
       return -1;
    }

    double min = numeric_limits<double>::max();
    double index = -1;
    for (unsigned int j = 0; j < intersections.size(); j++) {
      if (intersections.at(j) > 0 && intersections.at(j) < min) {
          min = intersections.at(j); //Getting the nearest object
          index = j;
      }
    }

    return index;
}

Color getColorAt(Vector intersectionPoint, Vector camera_ray_direction, long index_of_closest_object) {
    //Modelo de iluminação de Phong

    Primitive *closest_object = objects.at(index_of_closest_object); //getting the closest obj
    Vector n = closest_object->getNormalAt(intersectionPoint); //closest obj normal
    Material object_material = closest_object->getMaterial(); //getting its material

    Color color(0, 0, 0);
    //color = color.scale(ambientLight);
    for (unsigned int light_i = 0; light_i < light_sources.size(); light_i++) {
        Color light_color_a = light_sources.at(light_i)->col_a();
        Color light_color_d = light_sources.at(light_i)->col_d();
        Color light_color_s = light_sources.at(light_i)->col_s();
        Vector l = light_sources.at(light_i)->pos().add(intersectionPoint.negative()).normalize();
        float cossine = n.dotProduct(l);

        // Componente ambiente
        color = color.add(light_color_a.multiply(object_material.ka()));

        if (cossine > 0) {

            // Componente Difusa
            color = color.add(light_color_d.multiply(object_material.kd()).scale(cossine)); //Luz + luz difusa

            //Componente especular
            Vector v = camera_ray_direction.negative(); //Vetor do ray
            double _2ln = 2*l.dotProduct(n);
            Vector _2lnn = n.multiply(_2ln);
            Vector r = _2lnn.add(l.negative()).normalize();

            double specular = r.dotProduct(v);

            //if (specular > 0) {
                color = color.add(light_color_s.multiply(object_material.ks()).scale(pow(specular, object_material.m()))); //Multiplicando pela especular
            //}

        }
    }

    return color.clip(); //Normalizar cores
}

void addPrimitive(Primitive * primitive) {
    objects.push_back(primitive); //Adicionando primitivas básicas
}

void addObject(Object object) {
    vector<Primitive *> faces = object.getFaces();
    for(unsigned int i = 0; i < faces.size(); i++) {
        addPrimitive(faces.at(i));
    }
}

void addLight(Light &light) {
    light_sources.push_back(dynamic_cast<Light *>(&light));
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);

    //Ground:
    Plane ground(Vector(0,1,0), -3, brown);
    addPrimitive(&ground);
/*
//    Random objects:

//    //Torus

    Torus torus(red);
//    torus.scale(0.5,0.5,0.5);
//    torus.translate(0.75,1,0);
//    addObject(torus);


    //Windmll
    Wind wind(metal);
//    wind.scale(0.0Closet,0.0Closet,0.0Closet);
//    wind.rotateX(270);
//    addObject(wind);

    //Hexagono
    Hex hexagono(green);
    hexagono.scale(0.015,0.015,0.015);
//    hexagono.rotateX(90);
//    hexagono.translate(0,0.47,0); //quando hex esta deitado
    hexagono.rotateY(150);
    hexagono.translate(0,0.5,0.5);
    addObject(hexagono);

    //Hexagono 2
    Hex hexagono_2(brass);
    hexagono_2.scale(0.015,0.015,0.015);
//    hexagono_2.rotateX(90);
//    hexagono_2.translate(0,0.47,0); //quando hex esta deitado
    hexagono_2.rotateY(120);
    hexagono_2.translate(0,0.5,-0.5);
    addObject(hexagono_2);

    Mesa mesa(metal);
    addObject(mesa);
    mesa.rotateY(60);

    //Piso:
    Cubo piso(piso1);
    piso.scale(3, 0.1, 2.5);
    piso.translate(0,-1.3,-1);
    addObject(piso);
*/

    //laptop:
    Laptop notebook(silver);
    notebook.rotateY(-45);
    notebook.scale(0.005, 0.005, 0.005);
    notebook.translate(-1.45, 0.15, -2.75);
    addObject(notebook);

    //Televisao:
    Tv televisao(blackPlastic);
    televisao.scale(0.01,0.01,0.01);
    televisao.translate(0.25,0.5,-3.1);
    addObject(televisao);


    //Closet:
    Closet armario(copper);
    armario.scale(0.0035,0.0035,0.0035);
    armario.rotateY(270);
    armario.translate(2.2, 0, -1);
    addObject(armario);

    //Cadeira:
    Chair cadeira(gold);
    cadeira.scale(0.012,0.012,0.012);
    cadeira.rotateY(230);
    cadeira.translate(-1, -0.45, -2.35);
    addObject(cadeira);

    //Mesa:
    Desk mesa(copper);
    mesa.rotateY(90);
    mesa.scale(0.02,0.02,0.02);
    mesa.translate(-2.1,-0.5,-2.65);
    addObject(mesa);

    //Cama:
    Bed cama(jade);
    cama.scale(0.1,0.1,0.1);
    cama.rotateY(90);
    cama.translate(0.0, -0.25, 0.25);
    addObject(cama);

    //Criado mudo 1 (direita):
    Mesa cmudo1(ruby);
    cmudo1.scale(0.1, 0.3, 0.183);
    cmudo1.translate(1.2, 0.0, 0.9);
    addObject(cmudo1);

    //Criado mudo 2 (esquerda):
    Mesa cmudo2(greenPlastic);
    cmudo2.scale(0.1, 0.3, 0.183);
    cmudo2.translate(-0.4, 0.0, 0.9);
    addObject(cmudo2);

    //Luminaria mesa 1
    Lamp luminaria1(gold);
    luminaria1.scale(0.01, 0.01, 0.01);
    luminaria1.rotateY(230);
    luminaria1.translate(1.2, 0.0, 0.9);
    addObject(luminaria1);

    //Luminaria mesa 2
    Lamp luminaria2(gold);
    luminaria2.scale(0.01, 0.01, 0.01);
    luminaria2.rotateY(90);
    luminaria2.translate(-0.4, 0.0, 0.9);
    addObject(luminaria2);

    //Cenario
    Cenario quarto(silver);
    quarto.scale(0.75,0.5,0.75);
    quarto.rotateY(90);
    quarto.translate(0,-0.5,-1);
    addObject(quarto);
    double aspectRatio = (double)W / (double)H;

//    Vector camera_position(-6, 0.3, 3);
//    Vector camera_position(-6, 4, 3);
//    Vector camera_position(0, 4, 4);
    Vector camera_position(2, 5, 4); //visao de canto
//    Vector camera_position(8, 0, 1); //de lado
//    Vector camera_position(0, 3, 4); //de frente

    Vector look_at(0, 0, 0);
    Vector up(0, 1, 0);

    Camera camera(camera_position, look_at, up);

    Light light1(Vector(0, 0, 10), Color(1, 1, 1), white, white);
    Light light2(Vector(0, 10, 10), Color(1, 1, 1), white, white);

    addLight(light1);
    addLight(light2);

    QImage image = QImage(W, H, QImage::Format_RGB32);
    QGraphicsScene * graphic = new QGraphicsScene(this);

    double xamnt, yamnt;

    for (int j = 0; j < H; j++) {
        //Uma thread para cada linha da matriz de pixels
        #pragma omp parallel for
        for (int i = 0; i < W; i++) {

            if (W > H) {
                xamnt = ((i + 0.5)/W)*aspectRatio - ((W - H)/(double)H)/2;
                yamnt = ((H - j) + 0.5)/H;
            } else if (H > W){
                xamnt = (i + 0.5)/W;
                yamnt = (((H - j) + 0.5)/H)/aspectRatio - (((H - W)/(double)W)/2);
            } else {
                xamnt = (i + 0.5)/W;
                yamnt = ((H - j) + 0.5)/H;
            }

            Ray camera_ray = camera.createRay(xamnt, yamnt);

            vector<double> intersections;
            for (unsigned int index = 0; index < objects.size(); index++) {
                intersections.push_back(objects.at(index)->findIntersection(camera_ray));
            }

            long index_of_closest_object = getIndexOfClosestObject(intersections);

            Color c;
            if (index_of_closest_object < 0)
                c = black;
            else {
                Vector intersectionPoint = camera_ray.origin().add(camera_ray.direction().multiply(intersections.at(index_of_closest_object))); //MIT
                c = getColorAt(intersectionPoint, camera_ray.direction(), index_of_closest_object);
            }

            QRgb qtRGB = qRgb(c.r()*255, c.g()*255, c.b()*255);
            image.setPixel(i, j, qtRGB);
//            cout << i << " pixel " << j << endl;
         }
    }

    graphic->addPixmap(QPixmap::fromImage(image));
    ui->graphicsView->setScene(graphic);
}

MainWindow::~MainWindow()
{
    delete ui;
}
