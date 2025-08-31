#include <SoftwareSerial.h>
#include "functions.h"

// Pin definitions
const int fsrInstep = A0;   // FSR 1: Instep
const int fsrInside = A1;   // FSR 2: Inside

// buzzer
const int buzzer = 9;

// button （change if you need)
const int game1Button = 2;
const int game2Button = 3;
const int resetButton = 4;

// which game is on true is on (might not need)
// boolean g1state = false;
// boolean g2state = false;

// Bluetooth
const int BTrxPin = 10;
const int BTtxPin = 11;
SoftwareSerial BTSerial(BTrxPin,BTtxPin);  // Virtual port for bluetooth module; Rx, Tx pins

// Threshold for sensor activation (tune as needed)
// The sensor generates reverse readings, with smaller reading corresponding to higher force
const int threshold = 400;

// Game settings
const int sequenceLength = 3;  //// Decrease from 10 to 3 for faster debugging
int game1Sequence[sequenceLength];  // 0: instep, 1: inside

// Timing
unsigned long inputTimeout = 10000;  // Timeout per step in ms

void setup() {
  pinMode(game1Button,INPUT);
  pinMode(game2Button,INPUT);
  pinMode(resetButton,INPUT);
  Serial.begin(9600);
  BTSerial.begin(9600);
  randomSeed(analogRead(A5));  // Noise-based seed
  doublePrintln("Welcome to SkillSocc!");
}

void loop() {
  // char choice = Serial.read();
  // if (choice == '1') {
  //   buzzing(3);
  //   playGame1();
  // } else if (choice == '2') {
  //   buzzing(3);
  //   playGame2();
  // } else {
  //   Serial.println("Invalid input. Type '1' or '2'");  //// This error message is always shown once, no clue why
  // }
  // Serial.println("Choose a game: Type '1' or '2'");

  doublePrintln("Choose a game: Press Button '1' or '2'"); // type button
  unsigned long startTime = millis();
  int validInput = 0;

  while (millis() - startTime < inputTimeout){
    String BTInput = BTSerial.readString();
    BTInput.trim();  // Remove the line terminator, e.g. '\r''\n', to allow for String comparison

    if (digitalRead(game1Button) || BTInput == String("1")) {
    buzzing(3);
    playGame1();
    validInput = 1;
    } 
    else if (digitalRead(game2Button) || BTInput == String("2")) {
    buzzing(3);
    playGame2();
    validInput = 1;
    } 
    else if (digitalRead(resetButton) || BTInput == String("reset")) {
    doublePrintln("Game restart, please choose a game again!");
    validInput = 1;
    } 
    else if (BTInput.length() > 0) {
      doublePrintln("Invalid input.");
    }
    if (validInput) {
      break;
    }
  }
  if (!validInput) {  // Time-out
    doublePrintln("Invalid input.");
  }
}
