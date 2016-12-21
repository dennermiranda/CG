#include "gldisplaywidget.h"

#include <iostream>
using namespace std;
#import "bed2.h"
#import "tv.h"
#include "triangle.h"
#include "vector.h"

glDisplayWidget::glDisplayWidget(QWidget *parent) : QGLWidget(parent)
{
    setMinimumHeight(300);
    setMinimumWidth(400);

    backgroundColor[0] = 0.1;
    backgroundColor[1] = 0.1;
    backgroundColor[2] = 0.1;

    lmbPressed = false;
    mmbPressed = false;
    rmbPressed = false;

    mouseX = 0;
    mouseY = 0;

    rotX = 0;
    rotY = 0;
    rotZ = 0;

    transX = 0;
    transY = 0;

    zoom = -5;
}

glDisplayWidget::~glDisplayWidget() {}

void glDisplayWidget::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();

    switch (key) {
    case Qt::Key_R:
        resetView();
        break;
    default:
        break;
    }

    updateGL();
}

void glDisplayWidget::initializeGL()
{
    glClearColor(backgroundColor[0],
                 backgroundColor[1],
                 backgroundColor[2],1);


    glEnable(GL_DEPTH_TEST);

    initDisplay();
}

void glDisplayWidget::resizeGL(int w, int h)
{
    width = w;
    height = h;
    glViewport(0,0,w,h);
}

void glDisplayWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    double norm = max(minimumWidth(),minimumHeight());
    windowWidth = width/norm;
    windowHeight = height/norm;

    glBegin(GL_PROJECTION_MATRIX);
    glLoadIdentity();
    glFrustum(-windowWidth/20,windowWidth/20,-windowHeight/20,windowHeight/20,0.1,10000);
    glEnd();

    glPushMatrix();
    glTranslated(transX,transY,zoom);
    glRotated(rotX,1,0,0);
    glRotated(rotY,0,1,0);
    glRotated(rotZ,0,0,1);


    drawScene();
    glPopMatrix();


}

void glDisplayWidget::resetView()
{
    rotX = 0;
    rotY = 0;
    rotZ = 0;

    transX = 0;
    transY = 0;

    zoom = -10;
}

void glDisplayWidget::mousePressEvent(QMouseEvent *event)
{
    mouseX = event->x();
    mouseY = event->y();

    if (event->button() == Qt::LeftButton) lmbPressed = true;
    if (event->button() == Qt::MidButton) mmbPressed = true;
    if (event->button() == Qt::RightButton) rmbPressed = true;
}

void glDisplayWidget::mouseReleaseEvent(QMouseEvent *event)
{
    lmbPressed = mmbPressed = rmbPressed = false;
}

void glDisplayWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (rmbPressed) {
        transX += ((double)(event->x() - mouseX))/100;
        transY += -((double)(event->y() - mouseY))/100;
        mouseX = event->x();
        mouseY = event->y();
    }

    if (mmbPressed) {
        if (zoom - ((double)(event->y() - mouseY))/100 >= -50) {
            zoom -= ((double)(event->y() - mouseY))/100;
        }

        mouseY = event->y();
    }

    if (lmbPressed) {
        rotZ += ((double)(event->x() - mouseX))/5;
        rotX += ((double)(event->y() - mouseY))/5;
        mouseX = event->x();
        mouseY = event->y();
    }

    updateGL();
}

void glDisplayWidget::wheelEvent(QWheelEvent *event)
{
    if (zoom + event->delta()/500.0f >= -50) {
        zoom += event->delta()/500.0f;
    }

    updateGL();
}

void glDisplayWidget::initDisplay() {
    //glDisable(GL_LIGHTING);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    //glEnable(GL_LIGHT1);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);

    GLfloat mat_diffuse[] = { 1,1,1,1 };
    GLfloat mat_ambient[] = { 1,1,1,1 };

    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);

    GLfloat light1_position[] = { -1.0, -2.0, -3.0, 0.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, light1_position);
}

void glDisplayWidget::drawScene() {

    //cama
    glColor3f(1.0,0,0);
    for (int i=0;i<bed2NumVerts*3;i=i+9) {
        Vector vect1 = Vector(bed2Verts [i],bed2Verts [i+1],bed2Verts [i+2]);
        Vector vect2 = Vector(bed2Verts [i+3],bed2Verts [i+4],bed2Verts [i+5]);
        Vector vect3 = Vector(bed2Verts [i+6],bed2Verts [i+7],bed2Verts [i+8]);
        Triangle triangle = Triangle(vect1, vect2, vect3);
        Vector normal = triangle.getNormal();


        glBegin(GL_TRIANGLES);
        glNormal3f(normal.x(), normal.y(), normal.z());
        glVertex3f(bed2Verts [i],bed2Verts [i+1],bed2Verts [i+2]);
        glVertex3f(bed2Verts [i+3],bed2Verts [i+4],bed2Verts [i+5]);
        glVertex3f(bed2Verts [i+6],bed2Verts [i+7],bed2Verts [i+8]);
        glEnd();
    }

    //tv
    glPushMatrix();
    glTranslatef(-1, -1, -1);
    glRotatef(30, 0, 1, 0);
    glScalef(0.5, 0.5, 0.5);

    glColor3f(0,1.0,0);
    for (int i=0;i<tvNumVerts*3;i=i+9) {
        Vector vect1 = Vector(tvVerts [i], tvVerts [i+1], tvVerts [i+2]);
        Vector vect2 = Vector(tvVerts [i+3],tvVerts [i+4],tvVerts [i+5]);
        Vector vect3 = Vector(tvVerts [i+6],tvVerts [i+7],tvVerts [i+8]);
        Triangle triangle = Triangle(vect1, vect2, vect3);
        Vector normal = triangle.getNormal();


        glBegin(GL_TRIANGLES);
        glNormal3f(normal.x(), normal.y(), normal.z());
        glVertex3f(tvVerts [i], tvVerts [i+1], tvVerts [i+2]);
        glVertex3f(tvVerts [i+3],tvVerts [i+4],tvVerts [i+5]);
        glVertex3f(tvVerts [i+6],tvVerts [i+7],tvVerts [i+8]);
        glEnd();
    }
    glPopMatrix();

}

