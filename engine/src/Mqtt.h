#ifndef __MQTT_H__
#define __MQTT_H__

#include <iostream>
#include <cmath>

#include "Defines.h"
#include "Track.h"

using namespace std;

class Mqtt
{
public:
    Mqtt(Track ** tracks) :
        tracks(tracks)
    {

    }

    ~Mqtt()
    {

    }

    bool open()
    {
        return true;
    }

    void close()
    {

    }

private:
    Track ** tracks;
};

#endif // __MQTT_H__
