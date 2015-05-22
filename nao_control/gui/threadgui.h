#ifndef THREADGUI_H
#define THREADGUI_H

#include <QtGui>

#include <iostream>
#include <sys/time.h>

#include "gui.h"

#include "../shared.h"

#define cycle_gui 20 //miliseconds


class ThreadGui:public QThread
{
public:
    ThreadGui(Shared* sm);

private:
    Gui* gui;
    Shared* sm;
protected:
    void run();
};

#endif // THREADGUI_H
