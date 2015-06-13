#include "control.h"
#include <iostream>
#include <opencv/cv.h>

#include <Ice/Ice.h>
#include <jderobot/walker.h>
#include <jderobot/pose3d.h>
#include <IceUtil/IceUtil.h>
#include "pthread.h"
#include "../raked_search.h"

int cont;

Control::Control(Ice::CommunicatorPtr ic, Shared* sm) {
	this->ic = ic;
	this->sm = sm;
	this->connected = false;

cont = 0;
}

Control::~Control() {}

int Control::connectGazebo() {

std::cout << "aa" << std::endl;
	Ice::PropertiesPtr prop = ic->getProperties();

	
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

	while ((sm->nSearches() > 0) && (!sm->isStopped()) && (!sm->isPaused())) {

std::cout << "entra aqui" << std::endl;
		jderobot::WalkerDataPtr walkerdata  = new jderobot::WalkerData();

		//GENERACION DE PARAMETROS PARA LA CAMINATA crear clase de busqueda rastrillada
		//walkerdata = sm->getNextSearch();

		walkerdata = sm->getRandomSearch();
		int index = sm->getIndex();

std::cout << "se van a procesar #" << index  << std::endl;
std::cout << "param1: " << walkerdata->param1 << std::endl;
std::cout << "param2: " << walkerdata->param2 << std::endl;
std::cout << "param3: " << walkerdata->param3 << std::endl;
std::cout << "param4: " << walkerdata->param4 << std::endl;
std::cout << "param5: " << walkerdata->param5 << std::endl;
std::cout << "param6: " << walkerdata->param6 << std::endl;
std::cout << "param7: " << walkerdata->param7 << std::endl;
std::cout << "param8: " << walkerdata->param8 << std::endl;
std::cout << "param9: " << walkerdata->param9 << std::endl;
std::cout << "param10: " << walkerdata->param10 << std::endl;
std::cout << "-----------------------------------" << std::endl;

	   	walk->setParams(walkerdata);
	
		//PRUEBA DE CAMINATA
		walk->startWalk();

		jderobot::StadisticsDataPtr stads = new jderobot::StadisticsData();
		stads = walk->getStadistics();

		stads->id = index;
		stads->distance = std::sqrt( std::pow((stads->x1 - stads->x0), 2) + std::pow((stads->y1 - stads->y0), 2) );  
		stads->fitness = calculateFitness(stads);
		
		sm->setStads(stads);
		

std::cout << "estadisticas" << std::endl;
std::cout << "x0: " << stads->x0 << std::endl;
std::cout << "y0: " << stads->y0 << std::endl;
std::cout << "z0: " << stads->z0 << std::endl;
std::cout << "x1: " << stads->x1 << std::endl;
std::cout << "y1: " << stads->y1 << std::endl;
std::cout << "z1: " << stads->z1 << std::endl;
std::cout << "simTime: " << stads->simTime << std::endl;
std::cout << "distanceX: " << stads->distanceX << std::endl;
std::cout << "distanceY: " << stads->distanceY << std::endl;
std::cout << "desviation: " << stads->desviation << std::endl;
std::cout << "has fallen: " << stads->fallen << std::endl;
std::cout << "fitness: " << stads->fitness << std::endl;
std::cout << "-----------------------------------" << std::endl;


	}


}

float Control::calculateFitness(jderobot::StadisticsDataPtr stads) {

	float fitness = -1;
	float directionality = 0;

	// directionality. How much the robot has been moving in a straight line
	float y0 = stads->y0;
	float y1 = stads->y1;

	if (y0 == 0) {	// avoid division by 0
		y0 = y0 + 10;
		y1 = y1 + 10;
	}

	if (y0 - y1 < 0) {
		directionality = y0 / (y0 + (y1 - y0));
	}
	else {
		directionality = y0 / (y0 + (y0 - y1));
	}

	//avoid division by 0	
	if (stads->desviation == 0)
		stads->desviation = 0.0000001;

	if (stads->fallen != 0)
		fitness = 0;
	else 
		fitness = sqrt( pow((stads->distance), 3) / stads->simTime ) * sqrt( 1 / stads->desviation ) * directionality;

	std::cout << "distance: " << stads->distance << " error: " << stads->desviation << " directionality: " << directionality << " time: " << stads->simTime << std::endl;
	
	return fitness;

}

void Control::update() {
	
	//Connect with gazebo
	if (!connected) {
		if (sm->canConnect() && !sm->isStopped()) {
			this->connectGazebo();
			connected = true;
		}
	}else{
		// coger caminata generada de shared
		//mandarla a gazebo.
		// esperar a que gazebo termine (como????)
		// generar estadisticas
/*
		if (this->sm->nSearches() > 0) {
			jderobot::WalkerDataPtr candidate = new jderobot::WalkerData();
			candidate = this->sm->getNextSearch();
			walk->setParams(candidate);
			std::cout << "prueba no. " << cont << std::endl;
			walk->startWalk();
			jderobot::StadisticsDataPtr stats = walk->getStadistics();
		}
*/	
	}


	//comprobar si ha terminado la simulación y generar nuevos parametros de caminata
	/*
	if (sm->simulationDone()) {
		//generar nuevos parámetros
		sm->setParams(params[], true);
	}


	*/
}

