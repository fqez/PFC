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



    class Shared {
        
    public:

	Shared();
        virtual ~Shared();

	void connectGazebo(bool);
	bool canConnect();
        
        
        
    private:
        pthread_mutex_t mutex;

	bool connect;
        
    };

#endif /* defined(____shared__) */
