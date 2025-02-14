#include <Wire.h>

#define NUM_FOLLOWERS 2 // Adjust based on the number of VCS systems
const int FOLLOWER_ADDRESSES[NUM_FOLLOWERS] = {0x10, 0x11}; // I2C addresses

void setup() {
    Wire.begin();  // Initialize as Master
    Serial.begin(9600);
    Serial.println("Master initialized. Enter volume commands.");
}

void loop() {
    if (Serial.available()) {
        int followerID;
        int volume;

        Serial.println("Enter Follower ID (0 to NUM_FOLLOWERS-1): ");
        while (Serial.available() == 0) {} // Wait for input
        followerID = Serial.parseInt();
        
        Serial.println("Enter volume to dispense (CC): ");
        while (Serial.available() == 0) {} // Wait for input
        volume = Serial.parseInt();
        
        if (followerID >= 0 && followerID < NUM_FOLLOWERS) {
            sendVolumeCommand(FOLLOWER_ADDRESSES[followerID], volume);
        } else {
            Serial.println("Invalid Follower ID.");
        }
    }
}

// Send volume command to a specific follower
void sendVolumeCommand(int address, int volume) {
    Wire.beginTransmission(address);
    Wire.write(volume);
    Wire.endTransmission();
    Serial.print("Sent volume ");
    Serial.print(volume);
    Serial.print(" to Follower at address 0x");
    Serial.println(address, HEX);

    delay(100); // Wait for execution
}
