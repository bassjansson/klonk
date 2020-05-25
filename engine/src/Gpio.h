#ifndef __GPIO_H__
#define __GPIO_H__

#include <iostream>
#include <cmath>
#include <string>
#include <stdint.h>

// Needed for I2C port
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

// Needed for GPIO pins
// #include <wiringPi.h>

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

    bool start()
    {
        string fileName = "/dev/i2c-" + to_string(I2C_BUS_NUMBER);

        if ((i2cFile = open(fileName.c_str(), O_RDWR)) < 0)
        {
            cout << "[Gpio] Failed to open the i2c bus." << endl;
            return false;
        }

        if (ioctl(i2cFile, I2C_SLAVE, I2C_SLAVE_ADDRESS) < 0)
        {
            cout << "[Gpio] Failed to acquire bus access and/or talk to the i2c slave." << endl;
            return false;
        }

        // writeSensorValue(i2cFile, 0x00, 0); // Set something

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

    void stop()
    {
        Pt_Stop();
    }

private:
    bool writeSensorValue(int fd, byte reg, byte val)
    {
        const int size = 2;
        byte buf[size] = { reg, val };

        if (write(fd, buf, size) != size)
        {
            cout << "[Gpio] Failed to write to the i2c bus." << endl;
            return false;
        }

        return true;
    }

    bool readSensorValues(int fd, byte reg, byte * buf, int size)
    {
        if (write(fd, &reg, 1) != 1)
        {
            cout << "[Gpio] Failed to write to the i2c bus." << endl;
            return false;
        }

        if (read(fd, buf, size) != size)
        {
            cout << "[Gpio] Failed to read from the i2c bus." << endl;
            return false;
        }

        return true;
    }

    int bytePairToInt(byte * pair)
    {
        byte flip[2] = { pair[1], pair[0] };

        return ((int16_t *) flip)[0];
    }

    void ptCallbackMethod(PtTimestamp timeStamp)
    {
        readSensorValues(i2cFile, 0x3B, i2cBuffer, I2C_BUFFER_SIZE);

        float z    = bytePairToInt(i2cBuffer + 4) / 16384.0f;
        float freq = z * 100.0f + 350.0f;

        for (int i = 0; i < NUMBER_OF_TRACKS; ++i)
            tracks[i]->setFrequency(freq);
    }

    static void ptCallback(PtTimestamp timeStamp, void * userData)
    {
        ((Gpio *) userData)->ptCallbackMethod(timeStamp);
    }

    int i2cFile;
    byte i2cBuffer[I2C_BUFFER_SIZE];

    Track ** tracks;
};

#endif // __GPIO_H__
