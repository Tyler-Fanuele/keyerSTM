#pragma once

const int freq = 420;

// Here's a breakdown of the timing elements:

// Dot: The shortest unit of Morse code, lasting for one time unit. 
// Dash: Three times the length of a dot, or three time units. 
// Intra-character space: The space between the dots and dashes within a single character is one time unit, according to RoboBlocky. 
// Inter-character space: The space between letters within a word is three time units. 
// Inter-word space: The space between words is seven time units

const int WPM = 20;
const int shortSignalLengthMS = 60000 / (WPM * 50);
const int longSignalLengthMS = shortSignalLengthMS * 3;