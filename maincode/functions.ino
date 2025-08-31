void buzzing(int numOfSounds){
  for(int i = 0; i < numOfSounds; i++){
    tone(buzzer, 1000);
    delay(1000);
    noTone(buzzer);
    if (i != numOfSounds - 1){
      delay(1000);
    }
  }
}

// Helper to get sensor hit
int readKick() {
  int instepValue = analogRead(fsrInstep);
  int insideValue = analogRead(fsrInside);

  //// The sensor generates reverse readings, with smaller reading corresponding to higher force
  if (instepValue < threshold && insideValue < threshold) {
    doublePrintln("Error: Both sensors triggered. Use one at a time.");
    delay(1000);
    return -1;
  }

  if (instepValue < threshold) return 0;
  if (insideValue < threshold) return 1;

  return -1;  // No valid input
}

// Display kick type
String kickType(int code) {
  return code == 0 ? "INSTEP" : "INSIDE";
}

// Game 1: Memorize and repeat
void playGame1() {
  doublePrintln("Game 1: Memorize the kick sequence.");
  delay(1000);

  // Generate random sequence
  //// May need different or modifiable lengths for each mode
  for (int i = 0; i < sequenceLength; i++) {
    game1Sequence[i] = random(0, 2); // 0 or 1
    doublePrint(i + 1);
    doublePrint(": ");
    doublePrintln(kickType(game1Sequence[i]));
    delay(1000);
  }

  // Hide the sequence
  //// No code to hide the sequence here; this function may not be supported in Arduino
  doublePrintln("Now reproduce the sequence!");
  delay(2000);
  doublePrintln("Start kicking!");

  // User input
  for (int i = 0; i < sequenceLength; i++) {
    unsigned long startTime = millis();
    int kick = -1;
    while (millis() - startTime < inputTimeout) {
      kick = readKick();
      if (kick != -1) break;
    }

    if (kick == -1) {
      doublePrintln("Timeout! Failed.");
      return;
    }

    if (kick != game1Sequence[i]) {
      doublePrint("Wrong kick at step ");
      doublePrintln(i + 1);
      doublePrintln("Failed.");
      return;
    }

    doublePrint("Correct: ");
    doublePrintln(kickType(kick));
    delay(500);
  }

  doublePrintln("Win! You completed the sequence.");
}

// Game 2: Endless mode
void playGame2() {
  doublePrintln("Game 2: Endless mode. Follow the kicks.");
  delay(1000);
  
  int score = 0;
  while (true) {
    int expectedKick = random(0, 2);
    doublePrint("Kick: ");
    doublePrintln(kickType(expectedKick));

    unsigned long startTime = millis();
    int kick = -1;
    while (millis() - startTime < inputTimeout) {
      kick = readKick();
      if (kick != -1) break;
    }

    if (kick == -1) {
      doublePrintln("Timeout! Game over.");
      break;
    }

    if (kick != expectedKick) {
      doublePrintln("Wrong kick! Game over.");
      break;
    }

    doublePrintln("Correct!");
    score++;
    delay(200);  //// The delay should be short to receive the next instruction in time before the ball falls to the ground
  }

  doublePrint("Your score: ");
  doublePrintln(score);
}

template <typename T>
void doublePrint(T content) {
  Serial.print(content);
  BTSerial.print(content);
}

template <typename T>
void doublePrintln(T content) {
  Serial.println(content);
  BTSerial.println(content);
}