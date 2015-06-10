//
//  shared.h
//  
//
//  Created by Francisco Perez on 18/02/14.
//
//

#ifndef TELECAMERA_QT_SHARED_MEM
#define TELECAMERA_QT_SHARED_MEM

#include <QtGui>
#include <iostream>
#include <pthread.h>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <jderobot/walker.h>



    class Shared {
        
    public:

	Shared();
        virtual ~Shared();

	enum State {START, PAUSE, STOP};

	void connectGazebo(bool);
	bool canConnect();

	bool isRunning();
	bool isPaused();
	bool isStopped();

	void setSimState(int);
	void setSearchState(int);
	
	bool isSearchRunning();
	bool isSearchPaused();
	bool isSearchStopped();

	void addSearch(jderobot::WalkerData*);
        
        
        
    private:
        pthread_mutex_t mutex;

	bool connect;
	State searchState;
	bool pause;
	State state;
	std::vector<jderobot::WalkerDataPtr> searches;
        
    };

#endif /* defined(____shared__) */
