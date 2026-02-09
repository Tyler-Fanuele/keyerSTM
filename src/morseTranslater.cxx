#include <morseTranslater.h>
#include <string.h>

bool MorseTranslater::settleArray()
{
    if (arrayOverSize() == true)
    {
        clearArray();
        return true;
    }
    return false;
}

void MorseTranslater::clearArray()
{
    memset(_currentMorseArray, '\0', sizeof(_currentMorseArray));
    _currentIndex = 0;
}

void MorseTranslater::addDot() 
{
    settleArray();
    _currentMorseArray[_currentIndex++] = '.';
}

void MorseTranslater::addDash()
{
    settleArray();
    _currentMorseArray[_currentIndex++] = '-';
}

char MorseTranslater::translate() {
    const char temp = MorseBinaryTree::parseMorseCode(_currentMorseArray);
    clearArray();
    return temp;
}

MorseTranslater Translator;