#include  <speekerKeyPlayer.h>
#include <stdio.h>
#include <morseBinaryTree.h>

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

void SpeekerKeyPlayer::playTest(bool printTest)
{
    playStr("KEYER", 5, printTest, printTest);
}

void SpeekerKeyPlayer::playChar(char c, bool printInut, bool printOutput)
{
    if (printInut)
    {
        printf("%c'", c);
    }
    char morse[10] = {0};
    if (!MorseBinaryTree::encodeChar(c, morse, 6))
    {
        return;
    }

    for (int i = 0; i < 5; i++)
    {
        if (morse[i] == '\0')
        {
            break;
        }
        if (morse[i] == '-')
        {
            playLong();
            if (printOutput)
            {
                printf("%c", morse[i]);
            }
        }
        else if (morse[i] == '.')
        {
            playShort();
            if (printOutput)
            {
                printf("%c", morse[i]);
            }
        }
        
        if (i != 4)
        {
            HAL_Delay(_shortLength);
        }
    }

    if(printOutput)
    {
        printf("'");
    }
}

void SpeekerKeyPlayer::playStr(const char* str, size_t size, bool printInut ,bool printOutput)
{
    for (size_t i = 0; i < size; i++)
    {
        if (str[i] == '\0')
        {
            break;
        }
        playChar(str[i], printInut, printOutput);
        HAL_Delay(_longLength);
    }
}

void SpeekerKeyPlayer::playBeep(int msDelay)
{
    HAL_GPIO_WritePin(_speekerPort, _speekerPin, GPIO_PIN_SET);
    HAL_Delay(msDelay);
    HAL_GPIO_WritePin(_speekerPort, _speekerPin, GPIO_PIN_RESET);
}

SpeekerKeyPlayer SpeekerPlayer(BUZZER_PIN, BUZZER_PIN_PORT, shortSignalLengthMS, longSignalLengthMS);