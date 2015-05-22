#include "threadcontrol.h"

ThreadControl::ThreadControl(Ice::CommunicatorPtr ic, Shared* sm)
{
    this->control = new Control(ic, sm);
}

void ThreadControl::run()
{
    struct timeval a, b;
    long totalb, totala;
    long diff;

    while (true) {
        gettimeofday(&a, NULL);
        totala = a.tv_sec * 1000000 + a.tv_usec;

        this->control->update();

        gettimeofday(&b, NULL);
        totalb = b.tv_sec * 1000000 + b.tv_usec;
        diff = (totalb - totala) / 1000;

        if (diff < 0 || diff > cycle_control)
            diff = cycle_control;
        else
            diff = cycle_control - diff;


        /*Sleep Algorithm*/
        usleep(diff * 1000);
        if (diff < 33)
            usleep(33 * 1000);
    }
}
