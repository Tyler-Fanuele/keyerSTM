#include  <speekerKeyPlayer.h>


SpeekerKeyPlayer::SpeekerKeyPlayer(unsigned int speekerPin, GPIO_TypeDef* speekerPort, const unsigned int shortLength, const unsigned int longLength)
{
    _shortLength = shortLength;
    _longLength = longLength;

    _speekerPin = speekerPin;
    _speekerPort = speekerPort;
}

void SpeekerKeyPlayer::playShort()
{
    playBeep(_shortLength);
}

void SpeekerKeyPlayer::playLong()
{
    playBeep(_longLength);
}

void SpeekerKeyPlayer::playTest()
{
    // K
    playShort();
    HAL_Delay(_shortLength);
    playLong();
    HAL_Delay(_shortLength);
    playShort();
    HAL_Delay(shortSignalLengthMS);

    // E
    playShort();
    HAL_Delay(shortSignalLengthMS);

    // Y
    playShort();
    HAL_Delay(_shortLength);
    playLong();
    HAL_Delay(_shortLength);
    playShort();
    HAL_Delay(_shortLength);
    playShort();
    HAL_Delay(shortSignalLengthMS);

    // E
    playShort();
    HAL_Delay(shortSignalLengthMS);

    // R
    playLong();
    HAL_Delay(_shortLength);
    playShort();
    HAL_Delay(_shortLength);
    playLong();
    HAL_Delay(shortSignalLengthMS);
}

void SpeekerKeyPlayer::playBeep(int msDelay)
{
    HAL_GPIO_WritePin(_speekerPort, _speekerPin, GPIO_PIN_SET);
    HAL_Delay(msDelay);
    HAL_GPIO_WritePin(_speekerPort, _speekerPin, GPIO_PIN_RESET);
}

SpeekerKeyPlayer SpeekerPlayer(BUZZER_PIN, BUZZER_PIN_PORT, shortSignalLengthMS, longSignalLengthMS);