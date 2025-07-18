// Pin definitions
const int fsrInstep = A0;   // FSR 1: Instep
const int fsrInside = A1;   // FSR 2: Inside

// Threshold for sensor activation (tune as needed)
//// The sensor generates reverse readings, with smaller reading corresponding to higher force
const int threshold = 400;

// Game settings
const int sequenceLength = 3;  //// Decrease from 10 to 3 for faster debugging
int game1Sequence[sequenceLength];  // 0: instep, 1: inside

// Timing
unsigned long inputTimeout = 10000;  // Timeout per step in ms

// buzzer
const int buzzer = 9;

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
    Serial.println("Error: Both sensors triggered. Use one at a time.");
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
  Serial.println("Game 1: Memorize the kick sequence.");
  delay(1000);

  // Generate random sequence
  //// May need different or modifiable lengths for each mode
  for (int i = 0; i < sequenceLength; i++) {
    game1Sequence[i] = random(0, 2); // 0 or 1
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.println(kickType(game1Sequence[i]));
    delay(1000);
  }

  // Hide the sequence
  //// No code to hide the sequence here; this function may not be supported in Arduino
  Serial.println("Now reproduce the sequence!");
  delay(2000);
  Serial.println("Start kicking!");

  // User input
  for (int i = 0; i < sequenceLength; i++) {
    unsigned long startTime = millis();
    int kick = -1;
    while (millis() - startTime < inputTimeout) {
      kick = readKick();
      if (kick != -1) break;
    }

    if (kick == -1) {
      Serial.println("Timeout! Failed.");
      return;
    }

    if (kick != game1Sequence[i]) {
      Serial.print("Wrong kick at step ");
      Serial.println(i + 1);
      Serial.println("Failed.");
      return;
    }

    Serial.print("Correct: ");
    Serial.println(kickType(kick));
    delay(500);
  }

  Serial.println("Win! You completed the sequence.");
}

// Game 2: Endless mode
void playGame2() {
  Serial.println("Game 2: Endless mode. Follow the kicks.");
  delay(1000);
  
  int score = 0;
  while (true) {
    int expectedKick = random(0, 2);
    Serial.print("Kick: ");
    Serial.println(kickType(expectedKick));

    unsigned long startTime = millis();
    int kick = -1;
    while (millis() - startTime < inputTimeout) {
      kick = readKick();
      if (kick != -1) break;
    }

    if (kick == -1) {
      Serial.println("Timeout! Game over.");
      break;
    }

    if (kick != expectedKick) {
      Serial.println("Wrong kick! Game over.");
      break;
    }

    Serial.println("Correct!");
    score++;
    delay(200);  //// The delay should be short to receive the next instruction in time before the ball falls to the ground
  }

  Serial.print("Your score: ");
  Serial.println(score);
}

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(A5));  // Noise-based seed
  Serial.println("Welcome to SkillSocc!");
  Serial.println("Choose a game: Type '1' or '2'");
}

void loop() {
  if (Serial.available() > 0) {
    char choice = Serial.read();
    if (choice == '1') {
      buzzing(3);
      playGame1();
    } else if (choice == '2') {
      buzzing(3);
      playGame2();
    } else {
      Serial.println("Invalid input. Type '1' or '2'");  //// This error message is always shown once, no clue why
    }

    Serial.println("Choose a game: Type '1' or '2'");
  }
}
