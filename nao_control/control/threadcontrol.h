#ifndef THREADCONTROL_H
#define THREADCONTROL_H

#include <QtGui>

#include <Ice/Ice.h>

#include <iostream>
#include <sys/time.h>

#include "../shared.h"
#include "control.h"

#define cycle_control 20 //miliseconds

class ThreadControl:public QThread
{
public:
    ThreadControl(Ice::CommunicatorPtr ic, Shared* sm);

private:
    Control* control;

protected:
    void run();

};

#endif 
