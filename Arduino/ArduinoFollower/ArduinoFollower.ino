#include <Wire.h>

// Define hardware pins
#define dir 3
#define stp 2
#define MS3 6
#define MS2 5
#define MS1 4
#define EN  7

int steps = 3075;  // Steps per full syringe movement
float constant = steps / 50.0;  // Steps per 1 CC
int position = 0;
int maximum = 200;  // Max syringe capacity in CC

void setup() {
    Wire.begin(0x10); // Unique I2C Address (Change for other followers)
    Wire.onReceive(receiveEvent);
    
    pinMode(stp, OUTPUT);
    pinMode(dir, OUTPUT);
    pinMode(MS1, OUTPUT);
    pinMode(MS2, OUTPUT);
    pinMode(MS3, OUTPUT);
    pinMode(EN, OUTPUT);

    resetBEDPins();
    Serial.begin(9600);
    Serial.println("Follower ready, waiting for commands...");
}

void loop() {
    // Main logic handled in receiveEvent()
}

// Function to handle incoming I2C commands
void receiveEvent(int bytes) {
    if (Wire.available()) {
        int volume = Wire.read();
        Serial.print("Received volume: ");
        Serial.println(volume);
        movePlunger(volume);
    }
}

// Move the plunger to dispense/withdraw fluid
void movePlunger(int volume) {
    int targetPosition = position + volume * constant;

    if (targetPosition > maximum * constant || targetPosition < 0) {
        Serial.println("Requested volume exceeds syringe limits.");
        return;
    }

    digitalWrite(EN, LOW);
    
    if (volume > 0) {
        digitalWrite(dir, LOW); // Fill syringe
    } else {
        digitalWrite(dir, HIGH); // Empty syringe
        volume = -volume; // Convert to positive
    }

    for (int x = 0; x < volume * constant; x++) {
        digitalWrite(stp, HIGH);
        delay(1);
        digitalWrite(stp, LOW);
        delay(2);
    }

    position = targetPosition;
    Serial.print("New plunger position: ");
    Serial.println(position);
}

// Reset motor driver pins
void resetBEDPins() {
    digitalWrite(stp, LOW);
    digitalWrite(dir, LOW);
    digitalWrite(MS1, LOW);
    digitalWrite(MS2, LOW);
    digitalWrite(EN, HIGH);
}
