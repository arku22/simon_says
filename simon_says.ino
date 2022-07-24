//Simon Says game

// Array index for colors -> [0]:Red, [1]:Green, [2]:Blue, [3]:Yellow
int ledPins[] = {12, 11, 10, 9};
int buttonPins[] = {7, 6, 5, 4};
int ledFreqs[] = {262, 330, 392, 494};
int buzzerPin = 2;

bool gameStarted = false;
int roundCounter;
int gameSequence[10];

int buttonVal; // records the button that the player presses
unsigned long startTime;
int timeLimit = 10000;

void setup() {
  for (int i=0; i<4; i++) {
    pinMode(ledPins[i], INPUT);
    pinMode(buttonPins[i], INPUT_PULLUP);
  }

  Serial.begin(9600);

}

void loop() {

  // Check if game has started or not
  // start game if not started
  if (!gameStarted) {
    startGame();
    roundCounter = 0;
  }
  
  // if already started
  //  play the sequence for the current round
  for (int i=0; i<=roundCounter; i++) {
    flashLed(gameSequence[i]);
    delay(500);
    turnOffAllLeds();
    delay(200);
  }

  // Player Input
  for (int i=0; i<=roundCounter; i++) {
    startTime = millis();
    
    // Wait for player input until the timer runs out
    while (gameStarted == true) {
      buttonVal = buttonPushed();
      if (buttonVal < 4) {
        // button was pushed
        flashLed(buttonVal);
        delay(200);
        if (buttonVal != gameSequence[i]) {
          // incorrect button pushed, game over
          Serial.println("Incorrect...");
          playLoseSequence();
          break;
        }
        else {
          // correct button was pushed, go to next round
          delay(250);
          turnOffAllLeds();
          break;
        }        
      }
      else {
        // no button pushed
        if (millis() - startTime > timeLimit) {
          Serial.println("Time up...");
          playLoseSequence();
          break;
        }
      }      
    }
  }

  // go to next round
  roundCounter += 1;
  // All rounds over?
  // YES, then go to the next round otherwise simply go to the next round
  if (roundCounter == 10) {
    playWinSequence();
    turnOffAllLeds();
  }
  
  delay(1000);  
}

void startGame() {

  // Set gameStarted flag to true
  gameStarted = true;
  
  // Initialize game sequence
  randomSeed(analogRead(A1));

  Serial.println("---------------------Starting New Game!---------------------");
  Serial.print("Cheat- Game Sequence = ");
  for (int i=0; i<10; i++) {
    gameSequence[i] = random(0, 4);
    Serial.print(gameSequence[i]);
  }
  Serial.println();

  // play game start welcome sound
  //  play red led freq & blink all 4 leds 4 times
  
  for (int i=0; i<4; i++) {
    turnOnAllLeds();
    tone(buzzerPin, ledFreqs[0]);
    delay(200);
    turnOffAllLeds();
    delay(200);
  }
  noTone(buzzerPin);
  delay(2000);  

}

void turnOnAllLeds() {
  for (int i=0; i<4; i++) {
    digitalWrite(ledPins[i], HIGH);
  }
}

void turnOffAllLeds() {
  for (int i=0; i<4; i++) {
    digitalWrite(ledPins[i], LOW);
  }
  noTone(buzzerPin);
}

void flashLed(int pinIdx) {
  digitalWrite(ledPins[pinIdx], HIGH);
  tone(buzzerPin, ledFreqs[pinIdx]);
}

int buttonPushed() {
  if (digitalRead(buttonPins[0]) == LOW) {
    return 0;
  }  
  else if (digitalRead(buttonPins[1]) == LOW) {
    return 1;
  }
  else if (digitalRead(buttonPins[2]) == LOW) {
    return 2;
  }
  else if (digitalRead(buttonPins[3]) == LOW) {
    return 3;
  }
  else {
    return 4;
  }
}

void playWinSequence() {

  // turn on all leds
  turnOnAllLeds();

  // play winning sound
  tone(buzzerPin, 1318, 150);
  delay(175);
  tone(buzzerPin, 1567, 150);
  delay(175);
  tone(buzzerPin, 2637, 150);
  delay(175);
  tone(buzzerPin, 2093, 150);
  delay(175);
  tone(buzzerPin, 2349, 150);
  delay(175);
  tone(buzzerPin, 3135, 500);
  delay(500);

  // wait indefinitely for user to press any button to start a new game
  Serial.println("You Win!!! Press any button to start a new game...");
  while (gameStarted == true) {
    buttonVal = buttonPushed();
    // if user presses a button
    if (buttonVal < 4) {
      gameStarted = false;
    }
  }
}

void playLoseSequence() {
  
  // turn on all leds
  turnOnAllLeds();

  tone(buzzerPin, 130, 250);   //E6
  delay(275);
  tone(buzzerPin, 73, 250);   //G6
  delay(275);
  tone(buzzerPin, 65, 150);   //E7
  delay(175);
  tone(buzzerPin, 98, 500);   //C7
  delay(500);

  // wait indefinitely for user to press any button to start a new game
  Serial.println("Game Over! Press any button to start a new game...");
  while (gameStarted == true) {
    buttonVal = buttonPushed();
    // if user presses a button
    if (buttonVal < 4) {
      gameStarted = false;
    }
  }
}