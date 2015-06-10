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
	pthread_mutex_init(&mutex,NULL);
}

Shared::~Shared() {};

void Shared::connectGazebo(bool connect) {
	pthread_mutex_lock(&mutex);
	this->connect = connect;
	pthread_mutex_unlock(&mutex);
}

bool Shared::canConnect() {
	return connect;
}

bool Shared::isRunning() {
	return (this->state == START) ? true : false;
}

bool Shared::isPaused() {
	return (this->state == PAUSE) ? true : false;
}

bool Shared::isStopped() {
	return (this->state == STOP) ? true : false;
}

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

bool Shared::isSearchRunning() {
	return (this->searchState == START) ? true : false;
}

bool Shared::isSearchPaused() {
	return (this->searchState == PAUSE) ? true : false;
}

bool Shared::isSearchStopped() {
	return (this->searchState == STOP) ? true : false;
}
void Shared::addSearch(jderobot::WalkerData* search) {

	pthread_mutex_lock(&mutex);
	this->searches.push_back(search);
	pthread_mutex_unlock(&mutex);

}
