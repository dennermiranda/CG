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

using namespace std;

/* Globals */
vector<Primitive *> objects;
vector<Light *> light_sources;

double accuracy = 0.0000000001;

int W = 320*3;
int H = 240*3;

Color white(1.0, 1.0, 1.0);
Color black(0, 0, 0);

Material green(Color(0.2, 0.2, 0.2), Color(0.5, 1.0, 0.5), Color(0.5, 1.0, 0.5), 2);
Material brown(Color(0.2, 0.2, 0.2), Color(0.4, 0.2, 0.25), Color(0.1, 0.1, 0.1), 0);
Material metal(Color(0.2, 0.2, 0.2), Color(0.1, 0.1, 0.1), Color(1, 1, 1), 5);
Material red(Color(0.2, 0.2, 0.2), Color(1,0.1,0.1), Color(1, 1, 1), 20);
Material brass(Color(0.33, 0.22, 0.03), Color(0.78, 0.57, 0.11), Color(0.99, 0.91, 0.81), 27.8);

Vector X(1, 0, 0);
Vector Y(0, 1, 0);
Vector Z(0, 0, 1);
Vector O(0, 0, 0);

long  getIndexOfClosestObject(vector<double>  intersections) {
    // para se nao toca em nada
    if (intersections.size() == 0)
        return -1;

    if (intersections.size() == 1) {
        if (intersections.at(0) > 0)
            return 0;
       return -1;
    }

    double min = numeric_limits<double>::max();
    double index = -1;
    for (unsigned int j = 0; j < intersections.size(); j++) {
      if (intersections.at(j) > 0 && intersections.at(j) < min) {
          min = intersections.at(j);
          index = j;
      }
    }

    return index;
}

Color getColorAt(Vector intersectionPoint, Vector camera_ray_direction, long index_of_closest_object) {
    //Modelo de iluminação de Phong

    Primitive *closest_object = objects.at(index_of_closest_object);
    Vector n = closest_object->getNormalAt(intersectionPoint);
    Material object_material = closest_object->getMaterial();

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
            color = color.add(light_color_d.multiply(object_material.kd()).scale(cossine));

            //Componente especular
            Vector v = camera_ray_direction.negative();
            double _2ln = 2*l.dotProduct(n);
            Vector _2lnn = n.multiply(_2ln);
            Vector r = _2lnn.add(l.negative()).normalize();

            double specular = r.dotProduct(v);

            //if (specular > 0) {
                color = color.add(light_color_s.multiply(object_material.ks()).scale(pow(specular, object_material.m())));
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
//    wind.scale(0.03,0.03,0.03);
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

*/

    Bed bed(metal);
    bed.scale(0.03,0.03,0.03);
//    bed.rotateY(60);
    addObject(bed);

    double aspectRatio = (double)W / (double)H;

//    Vector camera_position(-6, 0.3, 3);
    Vector camera_position(-6, 4, 3); //Camera do Torus
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
