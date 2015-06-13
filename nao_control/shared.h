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

#include <fstream>
#include <iostream>

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
	jderobot::WalkerDataPtr getNextSearch();
	jderobot::WalkerDataPtr getSearch(int);
	jderobot::WalkerDataPtr getRandomSearch();
	int nSearches();


	void setStads(jderobot::StadisticsDataPtr);
	float getSimTime();
	std::string getFallen();
	float getX0();
	float getY0();
	float getZ0();
	float getX1();
	float getY1();
	float getZ1();
	float getDistance();
	int getFitness();
	float getDesviation();
	int getIndex();

	void setStadFlag(bool);
	jderobot::StadisticsDataPtr getStadFlag();
        
        
    private:
        pthread_mutex_t mutex;

	bool connect;
	State searchState;
	bool pause;
	State state;
	std::vector<jderobot::WalkerDataPtr> searches;
	std::vector<jderobot::StadisticsDataPtr> stadistics;	

    	int index_search;
	int index_stads;
	bool newStads;

    };

#endif /* defined(____shared__) */
