//
//  shared.cpp
//  
//
//  Created by Francisco Perez on 18/02/14.
//
//

#include "shared.h"
#include <cv.h>

Shared::Shared() {
	this->connect = false;
	this->pause = true;
	this->state = STOP;
	this->searchState = STOP;
	this->index_search = -1;
	this->index_stads = -1;
	this->newStads = false;
	std::ofstream stads_file( "stadistics.txt", std::ofstream::trunc );
	pthread_mutex_init(&mutex,NULL);
}

Shared::~Shared() {};

void Shared::connectGazebo(bool connect) {
	pthread_mutex_lock(&mutex);
	this->connect = connect;
	pthread_mutex_unlock(&mutex);
}

bool Shared::canConnect() { return connect; }

bool Shared::isRunning() { return (this->state == START) ? true : false; }

bool Shared::isPaused() { return (this->state == PAUSE) ? true : false; }

bool Shared::isStopped() { return (this->state == STOP) ? true : false; }

void Shared::setSimState(int s) {

	pthread_mutex_lock(&mutex);
	switch (s) {
		case 0:
			this->searchState = START;
			break;
		case 1:
			this->searchState = PAUSE;
			break;
		case 2:
			this->searchState = STOP;
			break;
	}
	pthread_mutex_unlock(&mutex);
}

void Shared::setSearchState(int s) {

	pthread_mutex_lock(&mutex);
	switch (s) {
		case 0:
			this->state = START;
			break;
		case 1:
			this->state = PAUSE;
			break;
		case 2:
			this->state = STOP;
			break;
	}
	pthread_mutex_unlock(&mutex);
}

bool Shared::isSearchRunning() { return (this->searchState == START) ? true : false; }

bool Shared::isSearchPaused() { return (this->searchState == PAUSE) ? true : false; }

bool Shared::isSearchStopped() { return (this->searchState == STOP) ? true : false; }

int Shared::nSearches() { return this->searches.size(); }

void Shared::addSearch(jderobot::WalkerData* search) {

	pthread_mutex_lock(&mutex);
	this->searches.push_back(search);
	pthread_mutex_unlock(&mutex);

}

jderobot::WalkerDataPtr Shared::getNextSearch() {
	if (!this->searches.empty()) {
		this->index_search++;
		return this->searches[index_search];
	}
	return 0;
}

jderobot::WalkerDataPtr Shared::getRandomSearch() {
	if (!this->searches.empty()) {
		int total = searches.size()-1;

	  	srand(time(NULL));
    	int rand = std::rand() % (total +1); // rand() return a number between ​0​ and RAND_MAX
		this->index_search = rand;
		jderobot::WalkerDataPtr data = this->searches[rand];
		searches.erase(searches.begin()+rand);
		return data;
	}
	return 0;
}

jderobot::WalkerDataPtr Shared::getSearch(int index) {
	if (!this->searches.empty() && this->searches.size() > index) {
		return this->searches[index];
	}
	return 0;
}

void Shared::setStads(jderobot::StadisticsDataPtr stads) {

	pthread_mutex_lock(&mutex);
	stadistics.push_back(stads);
	this->newStads = true;
	pthread_mutex_unlock(&mutex);

}

void Shared::setStadFlag(bool state) {
	pthread_mutex_lock(&mutex);
	this->newStads = state;
	pthread_mutex_unlock(&mutex);
}

int Shared::getIndex(){ return this->index_search; }

jderobot::StadisticsDataPtr Shared::getStadFlag() { 

	if (this->newStads) {
	
		this->index_stads++;

			if((this->stadistics[index_stads]->fitness/11) > 10) {	//if fitness > 10% (no fallings)
				std::ofstream stads_file;
				std::cout << "writing stadistics" << std::endl;
				stads_file.open( "stadistics.txt", std::ofstream::app );

				stads_file<<"Search_id#" << this->stadistics[index_stads]->id <<"\n";
				stads_file<<"Simulation time: " << this->stadistics[index_stads]->simTime << " s\n";
				stads_file<<"Origin: (" << this->stadistics[index_stads]->x0 << " cm, " << this->stadistics[index_stads]->y0 << " cm, " << this->stadistics[index_stads]->z0 <<" cm)\n";
				stads_file<<"End: (" << this->stadistics[index_stads]->x1 << " cm, " << this->stadistics[index_stads]->y1 << " cm, " << this->stadistics[index_stads]->z1 <<" cm)\n";
				stads_file<<"Distance: " << this->stadistics[index_stads]->distance << " cm\n";
				stads_file<<"Desviation: " << this->stadistics[index_stads]->desviation << " cm\n";
				stads_file<<"Fallen: " << this->stadistics[index_stads]->fallen << "\n";
				stads_file<<"Fitness: " << this->stadistics[index_stads]->fitness << "%\n\n";

				stads_file.close();
		}

		return stadistics[index_stads];
	}	

	return 0; 

}





        
