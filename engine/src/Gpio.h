#ifndef __GPIO_H__
#define __GPIO_H__

#include <iostream>
#include <cmath>
#include <wiringPi.h>

#include "porttime.h"
#include "Defines.h"
#include "Track.h"

using namespace std;

class Gpio
{
public:
    Gpio(Track ** tracks) :
        tracks(tracks)
    { }

    ~Gpio()
    { }

    bool open()
    {
        cout << "[Gpio] Starting GPIO timer callback." << endl;

        PtError ptError = Pt_Start(
            GPIO_TIMER_INTERVAL,
            &Gpio::ptCallback,
            this);

        if (ptError != ptNoError)
        {
            cout << "[Gpio] Failed to start GPIO timer callback." << endl;
            return false;
        }

        return true;
    }

    void close()
    {
        Pt_Stop();
    }

private:
    void ptCallbackMethod(PtTimestamp timeStamp)
    {
        cout << "GPIO callback called: " << timeStamp << endl;
    }

    static void ptCallback(PtTimestamp timeStamp, void * userData)
    {
        ((Gpio *) userData)->ptCallbackMethod(timeStamp);
    }

    Track ** tracks;
};

#endif // __GPIO_H__
