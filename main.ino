#define NUM_LEDS 4
#define NUM_BUTTONS 4
#define MAX_SEQUENCE 10

const int ledPins[NUM_LEDS] = {2, 3, 4, 5};  // LEDs
const int buttonPins[NUM_BUTTONS] = {6, 7, 8, 9};  // Buttons
const int buzzerPin = 10;

int sequence[MAX_SEQUENCE];  // Stores game sequence
int sequenceLength = 1;  // Starts with one step
int playerIndex = 0;  // Tracks player's progress

void setup() {
    randomSeed(analogRead(0)); // Random seed
    pinMode(buzzerPin, OUTPUT);
    
    // Setup LEDs & Buttons
    for (int i = 0; i < NUM_LEDS; i++) {
        pinMode(ledPins[i], OUTPUT);
        pinMode(buttonPins[i], INPUT_PULLUP);
    }

    delay(1000); // Start delay
    startGame();
}

void loop() {
    playSequence();  // Show current sequence
    if (!playerTurn()) {  // Player input
        gameOver();
    }
    delay(500);

    // Increase sequence and continue if successful
    if (sequenceLength < MAX_SEQUENCE) {
        sequenceLength++;
    } else {
        winGame();
    }
}

// Function to play the stored sequence
void playSequence() {
    for (int i = 0; i < sequenceLength; i++) {
        digitalWrite(ledPins[sequence[i]], HIGH);
        tone(buzzerPin, 500 + (sequence[i] * 100), 300);
        delay(500);
        digitalWrite(ledPins[sequence[i]], LOW);
        delay(300);
    }
}

// Function to check player input
bool playerTurn() {
    playerIndex = 0;
    while (playerIndex < sequenceLength) {
        int buttonPressed = waitForButtonPress();
        if (buttonPressed != sequence[playerIndex]) {
            return false;  // Wrong button pressed
        }
        playerIndex++;
    }
    return true;
}

// Function to wait for player input
int waitForButtonPress() {
    while (true) {
        for (int i = 0; i < NUM_BUTTONS; i++) {
            if (digitalRead(buttonPins[i]) == LOW) {
                digitalWrite(ledPins[i], HIGH);
                tone(buzzerPin, 500 + (i * 100), 200);
                delay(300);
                digitalWrite(ledPins[i], LOW);
                return i;
            }
        }
    }
}

// Start game and generate first sequence
void startGame() {
    for (int i = 0; i < MAX_SEQUENCE; i++) {
        sequence[i] = random(0, NUM_LEDS);
    }
    sequenceLength = 1;
}

// Function for Game Over
void gameOver() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < NUM_LEDS; j++) {
            digitalWrite(ledPins[j], HIGH);
        }
        tone(buzzerPin, 200, 500);
        delay(500);
        for (int j = 0; j < NUM_LEDS; j++) {
            digitalWrite(ledPins[j], LOW);
        }
        delay(500);
    }
    delay(1000);
    startGame(); // Restart the game
}

// Function for Winning the Game
void winGame() {
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < NUM_LEDS; j++) {
            digitalWrite(ledPins[j], HIGH);
        }
        tone(buzzerPin, 1000, 500);
        delay(500);
        for (int j = 0; j < NUM_LEDS; j++) {
            digitalWrite(ledPins[j], LOW);
        }
        delay(500);
    }
    delay(1000);
    startGame(); // Restart game after win
}
