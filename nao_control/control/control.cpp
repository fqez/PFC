#include "control.h"
#include <iostream>
#include <opencv/cv.h>

#include <Ice/Ice.h>
#include <jderobot/walker.h>
#include <jderobot/pose3d.h>
#include <IceUtil/IceUtil.h>
#include "pthread.h"
#include "../raked_search.h"


Control::Control(Ice::CommunicatorPtr ic, Shared* sm) {
	this->ic = ic;
	this->sm = sm;
	this->connected = false;
}

Control::~Control() {}

int Control::connectGazebo() {

std::cout << "aa" << std::endl;
	Ice::PropertiesPtr prop = ic->getProperties();

	jderobot::WalkerPrx walk;
	jderobot::Pose3DPrx pose3DIPrx;
	jderobot::Pose3DPrx pose3DOPrx;
std::cout << "bb" << std::endl;
	Ice::ObjectPrx walkprx = ic->propertyToProxy("walk.Walker.Proxy");
	if (0 == walkprx)
    	throw "Could not create proxy with pose3Dmotorstobilloder";
	// Cast to encoders
	walk = jderobot::WalkerPrx::checkedCast(walkprx);
	if (0 == walk)
    	throw "Invalid proxy tobilloder.Pose3DMotorsTobilloDer.Proxy";
std::cout << "cc" << std::endl;
	/*Ice::ObjectPrx pose3dIx = ic->propertyToProxy("walk.pose3DI.Proxy");
	if (pose3dIx == 0)
        	throw "Could not create proxy with motors";
	pose3DIPrx = jderobot::Pose3DPrx::checkedCast(pose3dIx);
    	if (pose3DIPrx == 0)
        	throw "Invalid proxy motors";
std::cout << "dd" << std::endl;
	Ice::ObjectPrx pose3dOx = ic->propertyToProxy("walk.pose3DO.Proxy");
	if (pose3dIx == 0)
        	throw "Could not create proxy with motors";
	pose3DOPrx = jderobot::Pose3DPrx::checkedCast(pose3dOx);
    	if (pose3DIPrx == 0)
        	throw "Invalid proxy motors";

std::cout << "ff" << std::endl;
	jderobot::Pose3DDataPtr p3ddata = new jderobot::Pose3DData();
	jderobot::Pose3DDataPtr data = pose3DOPrx->getPose3DData();

	//OBTENCION DE ESTADISTICAS DE POSICION (MEDIR DESVIACION CUADRATICA MEDIA PARA EJE Y)
    	std::cout << data->x << std::endl;
	std::cout << data->y << std::endl;
	std::cout << data->z << std::endl;
	std::cout << data->q0 << std::endl;
	std::cout << data->q1 << std::endl;
	std::cout << data->q2 << std::endl;
	std::cout << data->q3 << std::endl; */

	jderobot::WalkerData* walkerdata  = new jderobot::WalkerData();

	//GENERACION DE PARAMETROS PARA LA CAMINATA crear clase de busqueda rastrillada
    	walkerdata->param1 = 0.9;
	walkerdata->param2 = 11;
	walkerdata->param3 = -27;
	walkerdata->param4 = 11;
	walkerdata->param5 = 0;
	walkerdata->param6 = 42.1;
	walkerdata->param7 = 7.5;
	walkerdata->param8 = 0;
	walkerdata->param9 = -21.4;
	walkerdata->param10 = 2;

    	walk->setParams(walkerdata);
	
	//PRUEBA DE CAMINATA
    	walk->startWalk();

}

void Control::update() {
	
	//Connect with gazebo

	if (!connected) {
		if (sm->canConnect()) {
			this->connectGazebo();
			connected = true;
		}
	}


	//comprobar si ha terminado la simulación y generar nuevos parametros de caminata
	/*
	if (sm->simulationDone()) {
		//generar nuevos parámetros
		sm->setParams(params[], true);
	}


	*/
}

