#include <QtGui>
#include "glWidget.h"

#define MAXWORLD 50
#define v3f glVertex3f


GLWidget::GLWidget(QWidget *parent) : QGLWidget(parent) {
    setMouseTracking(true);


    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(10);

    timer -> start(10);
    this->setCam();

    this->radius = 15.0;
    this->lati = 0.2;
    this->longi = -1.0;
    this->old_x = 0.;
    this->old_y = 0.;

    this->step = 0; 
}

void GLWidget::setCam() {

    this->glcam_pos.x=0.;
    this->glcam_pos.y=0.;
    this->glcam_pos.z=0.;

    this->glcam_foa.x=10.;
    this->glcam_foa.y=0.;
    this->glcam_foa.z=0.;
}

void GLWidget::initializeGL() {
    
/* //Black background
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_COLOR_MATERIAL);
    glEnable(GL_BLEND);
    glEnable(GL_POLYGON_SMOOTH);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0, 0, 0, 0);
*/

    GLfloat ambient[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat diffuse[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat position[] = {0.0, 3.0, 3.0, 0.0};
    GLfloat lmodel_ambient[] = {0.2, 0.2, 0.2, 1.0};
    GLfloat local_view[] = {0.0};

    glViewport(0,0,800,600);  
    glDrawBuffer(GL_BACK);
    glClearColor(0.6f, 0.8f, 1.0f, 0.0f);
    glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* With this, the pioneer appears correctly, but the cubes don't */
    glLightfv (GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv (GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv (GL_LIGHT0, GL_POSITION, position);
    glLightModelfv (GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
    glLightModelfv (GL_LIGHT_MODEL_LOCAL_VIEWER, local_view);
    glEnable (GL_LIGHT0);
    // glEnable (GL_LIGHTING);

    glEnable(GL_TEXTURE_2D);     // Enable Texture Mapping
    glEnable (GL_AUTO_NORMAL);
    glEnable (GL_NORMALIZE);  
    glEnable(GL_DEPTH_TEST);     // Enables Depth Testing
    glDepthFunc(GL_LESS);  
    glShadeModel(GL_SMOOTH);     // Enables Smooth Color Shading
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GLWidget::resizeGL(int w, int h) {

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //glOrtho(0, w, 0, h, -1.0l, 1.0l); // set origin to bottom left corner
    gluPerspective(50.0, 	w/h, 	1.0, 50000.0);	
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt((double)this->glcam_pos.x, (double)this->glcam_pos.y, (double)this->glcam_pos.z, 
		(double)this->glcam_foa.x, (double)this->glcam_foa.y, (double)this->glcam_foa.z, 
		0., 0., 1.);
}

void GLWidget::paintGL() {


    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //floor
    glColor3f( 0, 0, 0 );
    glLineWidth(1.0f);
    glBegin(GL_LINES);
    for(int i=0;i<((int)MAXWORLD+1);i++) {
	v3f(-(int)MAXWORLD*10/2.+(float)i*10,-(int)MAXWORLD*10/2.,0.);
	v3f(-(int)MAXWORLD*10/2.+(float)i*10,(int)MAXWORLD*10/2., 0.);
	v3f(-(int)MAXWORLD*10/2.,-(int)MAXWORLD*10/2.+(float)i*10,0.);
	v3f((int)MAXWORLD*10/2.,-(int)MAXWORLD*10/2.+(float)i*10 ,0.);
    }
    glEnd();

    //absolute axis
    glLineWidth(3.0f);
    glColor3f( 0.7, 0., 0. );
    glBegin(GL_LINES);
    glVertex3f( 0.0, 0.0, 0.0 );   
    glVertex3f( 10.0, 0.0, 0.0 );
    glEnd();
    glColor3f( 0.,0.7,0. );
    glBegin(GL_LINES);
    glVertex3f( 0.0, 0.0, 0.0 );   
    glVertex3f( 0.0, 10.0, 0.0 );
    glEnd();
    glColor3f( 0.,0.,0.7 );
    glBegin(GL_LINES);
    glVertex3f( 0.0, 0.0, 0.0 );   
    glVertex3f( 0.0, 0.0, 10.0 );
    glEnd();
    glLineWidth(3.0f);

    //glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1,0,0);
    glPointSize(2);
    
   
   /*for (int i = 0; i < 30; i ++) {
    	for (int j = 0; j < 30; j++) {
            for (int rr = 0; rr < 30; rr++) {
    	    	glVertex3f(i, j, rr);
	    	glVertex3f(i, j, rr);
	    	glVertex3f(i, j, rr);
	    	glVertex3f(i, j, rr);
	    }
	}
    }*/
/*
    for (std::vector<jderobot::RGBPoint>::iterator it = cloud.begin(); it != cloud.end(); ++it){
	glColor3f(it->r, it->g, it->b);
	glBegin(GL_POINTS);
	glVertex3f(it->z, -it->x, -it->y);
	glEnd();
    }*/

    QMatrix4x4 rotY;
    QMatrix4x4 rotX;
    QVector3D aux;

    rotY.setToIdentity();
    rotY.rotate(90, 0.0, 1.0, 0.0);
    rotX.setToIdentity();
    rotX.rotate(-90, 1.0, 0.0, 0.0);

    for (std::vector<jderobot::RGBPoint>::iterator it = cloud.begin(); it != cloud.end(); ++it){

	aux.setX(it->x);
	aux.setY(it->y);
	aux.setZ(it->z);
	aux = rotX*rotY*aux;
	glColor3f(it->r, it->g, it->b);
	glBegin(GL_POINTS);
	glVertex3f(aux.x(), aux.y(), aux.z());
	glEnd();
    }
    
/*

    float x = 0.;
    float y = 0.;
    float z = 0.;
    int r = 0;
    int g = 0;
    int b = 0;
    int n = 0;
if (cloud.size() > 0) {
    for (int i = step; i < step + 10000; i ++) {
	n = i % cloud.size();
	x = cloud[n].x;
	y = cloud[n].y;
	z = cloud[n].z;
	r = cloud[n].r;
	g = cloud[n].g;
	b = cloud[n].b;
	
	glColor3f(r, g, b);
	glBegin(GL_POINTS);
	glVertex3f(x, y, z);
	glEnd();
    } 
    if (step >= cloud.size()) step = 0;
    else step += 10000;
}
	   
/*
    //Drawing vertex of a cube
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 5);
    glVertex3f(0, 5, 0);
    glVertex3f(5, 0, 0);
    glVertex3f(0, 5, 5);
    glVertex3f(5, 5, 0);
    glVertex3f(5, 0, 5);
    glVertex3f(5, 5, 5);
    */

    glEnd();
  
}

void GLWidget::expose() {

/*
std::cout << "CAM POSE: " << std::endl;
std::cout << "X: " << this->glcam_pos.x << std::endl;
std::cout << "Y: " << this->glcam_pos.y << std::endl;
std::cout << "Z: " << this->glcam_pos.z << std::endl;
std::cout << "FX: " << this->glcam_foa.x << std::endl;
std::cout << "FY: " << this->glcam_foa.y << std::endl;
std::cout << "FZ: " << this->glcam_foa.z << std::endl;
*/

    glDrawBuffer(GL_BACK);
    glClearColor(0.6f, 0.8f, 1.0f, 0.0f);

    glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    this->initializeGL();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    /*Angulo	ratio		znear, zfar*/
    gluPerspective(50.0, 	800/600, 	1.0, 50000.0);	

    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt((double)this->glcam_pos.x, (double)this->glcam_pos.y, (double)this->glcam_pos.z, 
		(double)this->glcam_foa.x, (double)this->glcam_foa.y, (double)this->glcam_foa.z, 
		0., 0., 1.);

}

void GLWidget::setCloud(std::vector<jderobot::RGBPoint> c) {
    cloud = c;
}


/* MOUSE LISTENERS */
void GLWidget::wheelEvent(QWheelEvent *wevent) {

    int numDegrees = wevent->delta() / 8;
    int numSteps = numDegrees / 15;

    /*this->glcam_pos.x = this->glcam_pos.x + 10;
    this->glcam_foa.x = this->glcam_foa.x + 10;*/
    
    if (numSteps > 0)
    	zoomIn();
    else 
	zoomOut();

    //qDebug() << wevent << "in my graphics view (wheelEvent).";
    wevent->accept();
}

void GLWidget::mousePressEvent(QMouseEvent *event) {

}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
    //printf("%d, %d\n", event->x(), event->y());

    float desp = 0.01;
    float x=event->x();
    float y=event->y();

    if  (event->buttons() & (Qt::LeftButton)) {

    	if ((x - old_x) > 0.0) longi -= desp;
	else if ((x - old_x) < 0.0) longi += desp;

	if ((y - old_y) > 0.0) lati += desp;
	else if ((y - old_y) < 0.0) lati -= desp;

	this->glcam_pos.x=radius*cosf(lati)*cosf(longi) + this->glcam_foa.x;
	this->glcam_pos.y=radius*cosf(lati)*sinf(longi) + this->glcam_foa.y;
	this->glcam_pos.z=radius*sinf(lati) + this->glcam_foa.z;

    }
    else if (event->buttons() & (Qt::RightButton)) {

	if ((x - old_x) > 0.0) longi -= desp;
	else if ((x - old_x) < 0.0) longi += desp;

	if ((y - old_y) > 0.0) lati += desp;
	else if ((y - old_y) < 0.0) lati -= desp;

	this->glcam_foa.x=-radius*cosf(lati)*cosf(longi) + this->glcam_pos.x;
	this->glcam_foa.y=-radius*cosf(lati)*sinf(longi) + this->glcam_pos.y;
	this->glcam_foa.z=-radius*sinf(lati) + this->glcam_pos.z;	
    }

    this->old_x = x;
    this->old_y = y;
    this->expose();
}

void GLWidget::zoomIn() {

	float vx, vy, vz;

	vx = (this->glcam_foa.x - this->glcam_pos.x)/15;
	vy = (this->glcam_foa.y - this->glcam_pos.y)/15;
	vz = (this->glcam_foa.z - this->glcam_pos.z)/15;

	this->glcam_foa.x = this->glcam_foa.x + vx;
	this->glcam_foa.y = this->glcam_foa.y + vy;
	this->glcam_foa.z = this->glcam_foa.z + vz;

	this->glcam_pos.x = this->glcam_pos.x + vx;
	this->glcam_pos.y = this->glcam_pos.y + vy;
	this->glcam_pos.z = this->glcam_pos.z + vz;

	this->expose();

}

void GLWidget::zoomOut() {

	float vx, vy, vz;

	vx = (this->glcam_foa.x - this->glcam_pos.x)/15;
	vy = (this->glcam_foa.y - this->glcam_pos.y)/15;
	vz = (this->glcam_foa.z - this->glcam_pos.z)/15;

	this->glcam_foa.x = this->glcam_foa.x - vx;
	this->glcam_foa.y = this->glcam_foa.y - vy;
	this->glcam_foa.z = this->glcam_foa.z - vz;

	this->glcam_pos.x = this->glcam_pos.x - vx;
	this->glcam_pos.y = this->glcam_pos.y - vy;
	this->glcam_pos.z = this->glcam_pos.z - vz;

        this->expose();

}

/* KEYBOARD LISTENERS */
void GLWidget::keyPressEvent(QKeyEvent* event) {
    switch(event->key()) {
    case Qt::Key_Up:
        std::cout << "Up pressed" << std::endl;
	zoomIn();
	break;
    case Qt::Key_Down:
	std::cout << "Down pressed" << std::endl;
	zoomOut();
	break;
    case Qt::Key_Escape:
        close();
        break;
    default:
        event->ignore();
        break;
    }
}
