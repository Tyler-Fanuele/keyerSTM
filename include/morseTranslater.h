#pragma once
#include <morseBinaryTree.h>

#define ARRAYSIZE 6

class MorseTranslater {

public:
    MorseTranslater() {}

    // Function to translate a Morse code sequence to a character
    char translate();

    // Function to add a dot to the current Morse code sequence
    void addDot();
    void addDash();

private:
    bool arrayOverSize() { return _currentIndex >= ARRAYSIZE; }
    bool settleArray();

    void clearArray();

    char _currentMorseArray[ARRAYSIZE] = {'\0', '\0', '\0', '\0', '\0', '\0'}; // Array to hold current morse code sequence
    unsigned int _currentIndex = 0;
};

extern MorseTranslater Translator;