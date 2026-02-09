#include  <speekerKeyPlayer.h>


SpeekerKeyPlayer::SpeekerKeyPlayer(unsigned int speekerPin, GPIO_TypeDef* speekerPort, const unsigned int shortLength, const unsigned int longLength)
{
    _shortLength = shortLength;
    _longLength = longLength;

    _speekerPin = speekerPin;
    _speekerPort = speekerPort;
}

void SpeekerKeyPlayer::sendShort()
{
    HAL_GPIO_WritePin(_speekerPort, _speekerPin, GPIO_PIN_SET);
    HAL_Delay(shortSignalLengthMS);
    HAL_GPIO_WritePin(_speekerPort, _speekerPin, GPIO_PIN_RESET);
}

void SpeekerKeyPlayer::sendLong()
{
    HAL_GPIO_WritePin(_speekerPort, _speekerPin, GPIO_PIN_SET);
    HAL_Delay(longSignalLengthMS);
    HAL_GPIO_WritePin(_speekerPort, _speekerPin, GPIO_PIN_RESET);
}

SpeekerKeyPlayer SpeekerPlayer(BUZZER_PIN, BUZZER_PIN_PORT, shortSignalLengthMS, longSignalLengthMS);