#ifndef _GLWIDGET_H
#define _GLWIDGET_H

#include <QtOpenGL/QGLWidget>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <cv.h>
#include <QDebug>

#include <jderobot/pointcloud.h>

class GLWidget : public QGLWidget {

    Q_OBJECT // must include this if you use Qt signals/slots

public:
    GLWidget(QWidget *parent = NULL);
    void expose();
    void setCloud(std::vector<jderobot::RGBPoint> c);

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void setCam();

    void zoomIn();
    void zoomOut();


private:

typedef struct SoRtype {
  struct SoRtype *father;
  float posx;
  float posy;
  float posz;
  float foax;
  float foay;
  float foaz;
  float roll;
} SofReference;

float robotx;
		float roboty;
		float robottheta;

   cv::Point3f glcam_pos;
   cv::Point3f glcam_foa;
   std::vector<jderobot::RGBPoint> cloud;


   float radius;
   float lati;
   float longi;
   float old_x;
   float old_y; 
   int step;

SofReference mypioneer;
};
#endif  /* _GLWIDGET_H */
