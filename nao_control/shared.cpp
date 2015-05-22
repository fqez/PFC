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
		pthread_mutex_init(&mutex,NULL);
	}

    Shared::~Shared() {};

	void Shared::connectGazebo(bool connect) {
std::cout << "1" << std::endl;
		pthread_mutex_lock(&mutex);
std::cout << "2" << std::endl;
		this->connect = connect;		
std::cout << "3" << std::endl;
		pthread_mutex_unlock(&mutex);
	}

	bool Shared::canConnect() {
		//mutex ??
		return connect;
	}
