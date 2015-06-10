#include <QtGui>
#include <Ice/Ice.h>
#include "gui/threadgui.h"
#include "gui/gui.h"
#include "control/threadcontrol.h"
#include "control/control.h"
#include <boost/algorithm/string.hpp>
#include "shared.h"
#include "gui/glWidget.h"


void exiting() {
	std::cout << "Exiting..." << std::endl;
	system("killall gzserver > /dev/null");
}

int main(int argc, char* argv[]) 
{

	QApplication a(argc, argv);

	Ice::CommunicatorPtr ic;
	try {
		ic = Ice::initialize(argc, argv);


		//Shared memory object
        	Shared* sm = new Shared();

        	//Create the threads for the gui and for the control and communication
       		ThreadGui* threadGui = new ThreadGui(sm);
        	ThreadControl* threadControl = new ThreadControl(ic, sm);

        	//Start the threads
        	threadGui->start();
        	threadControl->start();

	} catch (const Ice::Exception& ex) {
		std::cerr << ex << std::endl;
		exit(-1);
	} catch (const char* msg) {
		std::cerr << msg << std::endl;
		exit(-1);
	}

	std::atexit(exiting);
	return a.exec();
}

