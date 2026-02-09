#pragma once

#include <pinsLocations.h>
#include <keyerSettings.h>

class SpeekerKeyPlayer
{
public:

    SpeekerKeyPlayer();//(unsigned int speekerPin, unsigned int shortPin, unsigned int longPin, 
        //unsigned int freq, unsigned int shortLength, unsigned int longLength);

    void sendShort();
    void sendLong();

private:
    unsigned int _speekerPin;
    unsigned int _shortPin;
    unsigned int _longPin;

    unsigned int _freq;

    unsigned int _shortLength;
    unsigned int _longLength;

    SpeekerKeyPlayer(const SpeekerKeyPlayer&) = delete;
    SpeekerKeyPlayer& operator=(const SpeekerKeyPlayer&) = delete;
};

extern SpeekerKeyPlayer SpeekerPlayer;