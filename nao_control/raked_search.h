//
//  raked_search.h
//  
//
//  Created by Francisco Perez on 18/02/14.
//
//

#ifndef RAKED_SEARCH
#define RAKED_SEARCH

#include <QtGui>
#include <iostream>
#include <pthread.h>

#include <fstream>
#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include <iostream>
#include <opencv/cv.h>

#include <Ice/Ice.h>
#include <jderobot/walker.h>
#include <jderobot/pose3d.h>
#include <IceUtil/IceUtil.h>
#include "shared.h"



    class RakedSearch {
        
    public:

	RakedSearch(Shared*);
        virtual ~RakedSearch();
	
		//jderobot::WalkerDataPtr getLastSearch();
		//jderobot::WalkerDataPtr getSearch(int index);
		//jderobot::WalkerDataPtr getNextSearch();
		void initialize();
		void searchAll(bool);

		void resetIndex();
		void readIndex();
		void writeIndex();

		void writeSearch();

		bool checkPaused();
	
             
    private:

		struct props_t {
			float min;
			float max;
			float inc;
		};
	 
		struct params_t {
		    	props_t w;
			props_t hip_amplitude;
			props_t hip_shift;
		  	props_t knee_amplitude;
			props_t knee_phase;
			props_t knee_shift;
			props_t ankle_amplitude;
			props_t ankle_phase;
			props_t ankle_shift;
			props_t balance;
		};
		params_t my_params;
		    
		Shared* sm;
		pthread_mutex_t mutex;
		std::vector<params_t> searches;
		long index;

		int for1, for2, for3, for4, for5, for6, for7, for8, for9, for10;



	
    };

#endif 
