#ifndef CONTROL_H
#define CONTROL_H

#include <QtGui>
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "../shared.h"

#define X_IDEAL 200
#define Y_IDEAL 0

class Control 
{

	public:

		enum State {START, PAUSE, STOP};

		Control(Ice::CommunicatorPtr ic, Shared* sm);
		virtual ~Control();
		void update();
		int connectGazebo();
		void runn();
	
		float calculateFitness(jderobot::StadisticsDataPtr);

	private:
		//QMutex mutex;
		Ice::CommunicatorPtr ic;
		Shared* sm;
		bool connected;

		jderobot::WalkerPrx walk;
};

#endif 
