#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <Arduino.h>

void buzzing(int numOfSounds);
int readKick();
String kickType(int code);
void playGame1();
void playGame2();

template <typename T>
void doublePrint(T content);

template <typename T>
void doublePrintln(T content);

#endif