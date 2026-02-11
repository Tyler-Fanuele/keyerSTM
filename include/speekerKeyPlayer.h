#pragma once

#include <pinsLocations.h>
#include <keyerSettings.h>
#include "main.h"

class SpeekerKeyPlayer
{
public:

    SpeekerKeyPlayer(unsigned int speekerPin, GPIO_TypeDef* speekerPort, unsigned int shortLength, unsigned int longLength);

    void playShort();
    void playLong();
    void playTest();

private:
    void playBeep(int);

    unsigned int _speekerPin;
    GPIO_TypeDef* _speekerPort;

    unsigned int _shortLength;
    unsigned int _longLength;

    SpeekerKeyPlayer(const SpeekerKeyPlayer&) = delete;
    SpeekerKeyPlayer& operator=(const SpeekerKeyPlayer&) = delete;
};

extern SpeekerKeyPlayer SpeekerPlayer;