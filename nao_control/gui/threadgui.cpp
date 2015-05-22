#include "threadgui.h"
#include "glWidget.h"

ThreadGui::ThreadGui(Shared* sm)
{
    this->sm = sm;

    gui = new Gui(sm);
    //gui->show();

}

void ThreadGui::run()
{
    struct timeval a, b;
    long totalb, totala;
    long diff;

    while (true) {
        gettimeofday(&a, NULL);
        totala = a.tv_sec * 1000000 + a.tv_usec;

        gui->updateThreadGui();

        gettimeofday(&b, NULL);
        totalb = b.tv_sec * 1000000 + b.tv_usec;
        diff = (totalb - totala) / 1000;

        if (diff < 0 || diff > cycle_gui)
            diff = cycle_gui;
        else
            diff = cycle_gui - diff;


        /*Sleep Algorithm*/
        usleep(diff * 1000);
        if (diff < 33)
            usleep(33 * 1000);
    }
}
