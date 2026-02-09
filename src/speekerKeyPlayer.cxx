#include  <speekerKeyPlayer.h>
#include "main.h"


SpeekerKeyPlayer::SpeekerKeyPlayer()//(unsigned int speekerPin, unsigned int shortPin, unsigned int longPin, 
//        unsigned int freq, unsigned int shortLength, unsigned int longLength)
{
    // _speekerPin = speekerPin;
    // _shortPin = shortPin;
    // _longPin = longPin;

    // _freq = freq;

    // _shortLength = shortLength;
    // _longLength = longLength;

    //pinMode(speekerPin, OUTPUT); // Set buzzer - pin 9 as an output
    //pinMode(shortPin, INPUT_PULLUP);
    //pinMode(longPin, INPUT_PULLUP);
}

void SpeekerKeyPlayer::sendShort()
{
    HAL_GPIO_WritePin(BUZZER_PIN_PORT, BUZZER_PIN, GPIO_PIN_SET);
    HAL_Delay(shortSignalLengthMS);
    HAL_GPIO_WritePin(BUZZER_PIN_PORT, BUZZER_PIN, GPIO_PIN_RESET);
}

void SpeekerKeyPlayer::sendLong()
{
    HAL_GPIO_WritePin(BUZZER_PIN_PORT, BUZZER_PIN, GPIO_PIN_SET);
    HAL_Delay(longSignalLengthMS);
    HAL_GPIO_WritePin(BUZZER_PIN_PORT, BUZZER_PIN, GPIO_PIN_RESET);
}

SpeekerKeyPlayer SpeekerPlayer;//(buzzer, tip, ring2, freq, shortSignalLengthMS, longSignalLengthMS);