#ifndef CONTROL_H
#define CONTROL_H

#include <QtGui>
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "../shared.h"

class Control 
{

	public:

		enum State {START, PAUSE, STOP};

		Control(Ice::CommunicatorPtr ic, Shared* sm);
		virtual ~Control();
		void update();
		int connectGazebo();

	private:
		//QMutex mutex;
		Ice::CommunicatorPtr ic;
		Shared* sm;
		bool connected;
};

#endif 
